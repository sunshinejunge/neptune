#include "rename_file_message.h"

namespace neptune {
namespace dfs {


using namespace neptune::dfs;

RenameFileMessage::RenameFileMessage() :
  option_flag_(0), version_(0), lease_id_(INVALID_LEASE_ID)
{
  _packetHeader._pcode = RENAME_FILE_MESSAGE;
  memset(&rename_file_info_, 0, sizeof(RenameFileInfo));
}

RenameFileMessage::~RenameFileMessage()
{
}

int RenameFileMessage::deserialize(Stream& input)
{
  int64_t pos  = 0;
  int32_t iret = rename_file_info_.deserialize(input.get_data(), input.get_data_length(), pos);
  if (SUCCESS == iret)
  {
    input.drain(rename_file_info_.length());
  }
  if (SUCCESS == iret)
  {
    iret = input.get_vint64(ds_);
  }
  if (SUCCESS == iret)
  {
    iret = input.get_int32(&option_flag_);
  }
  if (SUCCESS == iret)
  {
    BasePacket::parse_special_ds(ds_, version_, lease_id_);
  }
  return iret;
}

int64_t RenameFileMessage::length() const
{
  int64_t len = rename_file_info_.length() + Serialization::get_vint64_length(ds_) + INT_SIZE;
  if (has_lease())
  {
    len += INT64_SIZE * 3;
  }
  return len;
}

int RenameFileMessage::serialize(Stream& output) const
{
  if (has_lease())
  {
    ds_.push_back(ULONG_LONG_MAX);
    ds_.push_back(static_cast<uint64_t> (version_));
    ds_.push_back(static_cast<uint64_t> (lease_id_));
  }

  int64_t pos  = 0;
  int32_t iret = rename_file_info_.serialize(output.get_free(), output.get_free_length(), pos);
  if (SUCCESS == iret)
  {
    output.pour(rename_file_info_.length());
    iret = output.set_vint64(ds_);
  }
  if (SUCCESS == iret)
  {
    iret = output.set_int32(option_flag_);
  }
  if (SUCCESS == iret)
  {
    BasePacket::parse_special_ds(ds_, version_, lease_id_);
  }
  return iret;
}


} //namespace dfs
} //namespace neptune
