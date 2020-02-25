#include "base/thread/new/ThreadIdNameManager.h"
#include "base/thread/new/PosixThread.h"
#include "base/thread/new/Thread.h"
#include <gtest/gtest.h>


namespace base {

const char kAThread[] = "a thread";
const char kBThread[] = "b thread";

TEST(test, AddThreads) {
  // base::ThreadIdNameManager* manager = base::ThreadIdNameManager::GetInstance();
  // base::Thread thread_a(kAThread);
  // base::Thread thread_b(kBThread);

//  thread_a.StartAndWaitForTesting();
//  thread_b.StartAndWaitForTesting();

  // EXPECT_STREQ(kAThread, manager->GetName(thread_a.GetThreadId()));
  // EXPECT_STREQ(kBThread, manager->GetName(thread_b.GetThreadId()));

  // thread_b.Stop();
  // thread_a.Stop();
}

/*
TEST_F(ThreadIdNameManagerTest, RemoveThreads) {
  base::ThreadIdNameManager* manager = base::ThreadIdNameManager::GetInstance();
  base::Thread thread_a(kAThread);

  thread_a.StartAndWaitForTesting();
  {
    base::Thread thread_b(kBThread);
    thread_b.StartAndWaitForTesting();
    thread_b.Stop();
  }
  EXPECT_STREQ(kAThread, manager->GetName(thread_a.GetThreadId()));

  thread_a.Stop();
  EXPECT_STREQ("", manager->GetName(thread_a.GetThreadId()));
}

TEST_F(ThreadIdNameManagerTest, RestartThread) {
  base::ThreadIdNameManager* manager = base::ThreadIdNameManager::GetInstance();
  base::Thread thread_a(kAThread);

  thread_a.StartAndWaitForTesting();
  base::PlatformThreadId a_id = thread_a.GetThreadId();
  EXPECT_STREQ(kAThread, manager->GetName(a_id));
  thread_a.Stop();

  thread_a.StartAndWaitForTesting();
  EXPECT_STREQ("", manager->GetName(a_id));
  EXPECT_STREQ(kAThread, manager->GetName(thread_a.GetThreadId()));
  thread_a.Stop();
}

TEST_F(ThreadIdNameManagerTest, ThreadNameInterning) {
  base::ThreadIdNameManager* manager = base::ThreadIdNameManager::GetInstance();

  base::PlatformThreadId a_id = base::PlatformThread::CurrentId();
  base::PlatformThread::SetName("First Name");
  std::string version = manager->GetName(a_id);

  base::PlatformThread::SetName("New name");
  EXPECT_NE(version, manager->GetName(a_id));
  base::PlatformThread::SetName("");
}

TEST_F(ThreadIdNameManagerTest, ResettingNameKeepsCorrectInternedValue) {
  base::ThreadIdNameManager* manager = base::ThreadIdNameManager::GetInstance();

  base::PlatformThreadId a_id = base::PlatformThread::CurrentId();
  base::PlatformThread::SetName("Test Name");
  std::string version = manager->GetName(a_id);

  base::PlatformThread::SetName("New name");
  EXPECT_NE(version, manager->GetName(a_id));

  base::PlatformThread::SetName("Test Name");
  EXPECT_EQ(version, manager->GetName(a_id));

  base::PlatformThread::SetName("");
}*/

}  // namespace
