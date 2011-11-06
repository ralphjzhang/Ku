/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "buffer.hpp"
#include "logger.hpp"

namespace ku { namespace log {

Logger::Logger() : writer_(*this)
{
  // Initial free buffer space pool, ideally it can be WriteBlock * number-of-work-threads
  free_buffer_.reserve(WriteBlock);
  // write_buffer_.reserve(WriteBlock);
  writer_.start();
}

Logger::~Logger()
{
  flush();
  writer_.quit();
}

void Logger::submit(Buffer& buf)
{
  std::unique_lock<std::mutex> lock(write_buffer_mutex_);
  write_buffer_.combine_buffer(buf);
  // Ask writer thread to write if we've gathered enough data
  if (write_buffer_.size() >= WriteBlock)
    writer_.submit(write_buffer_);
}

void Logger::give_back(Buffer& buf)
{
  buf.reclaim();
  std::lock_guard<std::mutex> lock(free_buffer_mutex_);
  free_buffer_.combine_buffer(buf);
}

void Logger::flush()
{
  submit(write_buffer_);
}

Logger& logger()
{
  // TODO this should be thread_local
  static /*thread_local*/ Logger lg;
  return lg;
}

} } // namespace ku::log

