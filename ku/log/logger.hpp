/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <mutex>
#include "writer.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

class Buffer;

class Logger
{
  friend class Collector;

public:
  Logger() { buffer_.reserve(1024); }
  ~Logger() { /*submit(buffer()); */ writer_.write(buffer_); }

  Writer& writer() { return writer_; }

  void submit(Buffer& buf);

private:
  Buffer& buffer() { return buffer_; }

  Writer writer_;
  Buffer buffer_;
  std::mutex mutex_;
};

Logger& logger();

} } // namespace ku::log

