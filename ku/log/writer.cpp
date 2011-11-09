/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "buffer.hpp"
#include "logger.hpp"
#include "writer.hpp"
#include <iostream>

namespace ku { namespace log {

void Writer::start()
{
  std::thread(std::ref(*this)).swap(thread_);
}

void Writer::operator () ()
{
  /*
  std::unique_lock<std::mutex> lock(mutex_);
  while (!quit_ && buffer_.empty())
    condition_.wait(lock);
  std::cout << "writer::operator()" << std::endl;
  for (auto& sink_ptr : sink_list_) {
    if (buffer_.log_level() >= sink_ptr->log_level())
      sink_ptr->write(buffer_);
  }
  logger_.give_back(buffer_);
  */
}

void Writer::submit(Buffer& buf)
{
  std::lock_guard<std::mutex> lock(mutex_);
  buffer_.swap(buf);
  condition_.notify_one();
}

} } // namespace ku::log

