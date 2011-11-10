/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <memory>
#include "log_level.hpp"
#include "util.hpp"

namespace ku { namespace log {

class BufferQueue;

class Sink;
typedef std::unique_ptr<Sink> Sink_ptr;

class Sink : private util::noncopyable
{
public:
  Sink() : log_level_(LogLevel::Debug) { }

  virtual ~Sink() { }

  virtual void write(BufferQueue const& queue) = 0;

  LogLevel log_level() { return log_level_; }
  void set_log_level(LogLevel log_level) { log_level_ = log_level; }

private:
  LogLevel log_level_;
};

} } // namespace ku::log

