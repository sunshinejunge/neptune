#include <climits>
#include <exception>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <bits/time.h>
#include "ThreadDetail.h"
#include "base/time/Time.h"
#include "ThreadException.h"
#include "base/concurrent/Cond.h"
#include "base/include/PublicDefine.h"

using namespace std;

namespace neptune {
namespace base {

Thread::Thread() :
  _running(false),
  _started(false),
  _detachable(false),
  _thread(0)
{
}

Thread::~Thread()
{
}

extern "C" {

static void* startHook(void* arg) {
  ThreadPtr thread;
  try
  {
    Thread* rawThread = static_cast<Thread*>(arg);
    thread = rawThread;
    rawThread->__decRef();
    thread->run();
  }
  catch(...)
  {
    std::terminate();
  }
  thread->_done();
  return 0;
}

} //extern "C

int Thread::start(size_t stackSize)
{
  ThreadPtr keepMe = this;
  Mutex::Lock sync(_mutex);

  if(_started)
  {
#ifdef _NO_EXCEPTION
    JUST_RETURN( _started == true , -1 );
    LOG(ERROR,"%s","ThreadStartedException");
#else
    throw ThreadStartedException(__FILE__, __LINE__);
#endif
  }

  __incRef();

  if(stackSize > 0)
  {
    pthread_attr_t attr;
    int rt = pthread_attr_init(&attr); 
#ifdef _NO_EXCEPTION
    if ( 0 != rt )
    {
      __decRef();
      LOG(ERROR,"%s","ThreadSyscallException");
      return -1;
    }
#else
    if ( 0 != rt )
    {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif

    if(stackSize < PTHREAD_STACK_MIN)
    {
      stackSize = PTHREAD_STACK_MIN;
    }

    rt = pthread_attr_setstacksize(&attr, stackSize);
#ifdef _NO_EXCEPTION
    if ( 0 != rt )
    {
      __decRef();
      LOG(ERROR,"%s","ThreadSyscallException");
      return -1;
    }
#else
    if( 0 != rt )
    {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif
    rt = pthread_create(&_thread, &attr, startHook, this);
#ifdef _NO_EXCEPTION
    if ( 0 != rt )
    {
      __decRef();
      LOG(ERROR,"%s","ThreadSyscallException");
      return -1;
    }
#else
    if( 0 != rt )
    {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
#endif
  }
  else
  {
    const int rt = pthread_create(&_thread, 0, startHook, this);
  #ifdef _NO_EXCEPTION
    if ( 0 != rt )
    {
      __decRef();
      LOG(ERROR,"%s","ThreadSyscallException");
      return -1;
    }
  #else
    if( 0 != rt ) 
    {
      __decRef();
      throw ThreadSyscallException(__FILE__, __LINE__, rt);
    }
  #endif
  }

  if ( _detachable )
  {
      detach();
  }
  _started = true;
  _running = true;
  return 0; 
}

bool Thread::isAlive() const 
{
  return _running;
}

void Thread::_done()
{
  Mutex::Lock lock(_mutex);
  _running = false;
}

int Thread::join()
{
  if(_detachable)
  {
#ifdef _NO_EXCEPTION
    LOG(ERROR,"%s","BadThreadControlException");
    JUST_RETURN( _detachable==true , -1);
#else
      throw BadThreadControlException(__FILE__, __LINE__);
#endif
  }

  const int rt = pthread_join(_thread, NULL);
#ifdef _NO_EXCEPTION
  if ( 0 != rt )
  {
    LOG(ERROR,"%s","ThreadSyscallException");
    return -1;
  }
#else
  if( 0 != rt )
  {
    throw ThreadSyscallException(__FILE__, __LINE__, rt);
  }
#endif
  return 0;
}

int Thread::detach()
{
  if(!_detachable)
  {
#ifdef _NO_EXCEPTION
    LOG(ERROR,"%s","BadThreadControlException");
    JUST_RETURN( _detachable==false, -1 );
#else
    throw BadThreadControlException(__FILE__, __LINE__);
#endif
  }

  const int rt = pthread_detach(_thread);
#ifdef _NO_EXCEPTION
  if ( 0 != rt )
  {
    LOG(ERROR,"%s","ThreadSyscallException");
    return -1;
  }
#else
  if(  0 != rt ) 
  {
    throw ThreadSyscallException(__FILE__, __LINE__, rt);
  }
#endif
  return 0;
}


pthread_t Thread::id() const
{
  return _thread;;
}

void Thread::ssleep(const Time& timeout)
{
  struct timeval tv = timeout;
  struct timespec ts;
  ts.tv_sec = tv.tv_sec;
  ts.tv_nsec = tv.tv_usec * 1000L;
  nanosleep(&ts, 0);
}

void Thread::yield()
{
  sched_yield();
}

} //namespace base
} //namespace neptune