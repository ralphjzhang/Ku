/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <iomanip>
#include "buffer_list.hpp"
#include "file_sink.hpp"

namespace ku { namespace log {

FileSink::FileSink(char const* path, char const* base_name, LogLevel log_level)
  : Sink(log_level), seq_no_(0), path_(path), base_name_(base_name)
  , size_(0), size_limit_(512 << 20) // 512 MB
{
  open();
}

void FileSink::write(BufferList const& list)
{
  size_ += ::writev(file_handle_, list.raw_buffer(), list.raw_buffer_count());
  if (size_ >= size_limit_) {
    close();
    rotate();
    open();
    size_ = 0;
  }
}

std::string FileSink::file_name(int seq_no) const
{
  time_t raw_time;
  time(&raw_time);
  tm t;
  localtime_r(&raw_time, &t);
  char buf[10];
  strftime(buf, 10, "%Y%m%d", &t);
  std::stringstream ss;
  ss << path_ << '/' << base_name_ << '_' << buf << '_' << getpid();
  if (seq_no) {
    ss << '.' << std::right << std::setfill('0') << std::setw(3) << seq_no;
  }
  ss << ".log";
  return std::move(ss.str());
}

void FileSink::open()
{
  file_handle_ = ::open(file_name().c_str(), O_CREAT | O_WRONLY | O_CLOEXEC, 0777);
  assert(file_handle_ > 0);
  if (file_handle_ < 0)
    file_handle_ = 0;
}

void FileSink::close()
{
  if (file_handle_) {
    ::fsync(file_handle_);
    ::close(file_handle_);
  }
}

void FileSink::rotate()
{
  rename(file_name().c_str(), file_name(++seq_no_).c_str());
}

} } // namespace ku::log

