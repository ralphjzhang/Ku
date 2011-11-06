/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <mutex>
#include <thread>
#include "writer.hpp"
#include "buffer.hpp"
#include "collector.hpp"

namespace ku { namespace log {

class Buffer;

class Logger
{
public:
  Logger();
  ~Logger();

  void add_sink(Sink_ptr sink) { writer_.add_sink(std::move(sink)); }

  Collector create_collector()
  {
    std::lock_guard<std::mutex> lock(free_buffer_mutex_);
    return Collector(free_buffer_, *this);
  }

  void submit(Buffer& buf);
  void give_back(Buffer& buf);

private:
  void flush();

private:
  const static size_t WriteBlock = 1024;//4096;

  Buffer write_buffer_, free_buffer_;
  std::mutex write_buffer_mutex_, free_buffer_mutex_;
  Writer writer_;
};

Logger& logger();

} } // namespace ku::log

