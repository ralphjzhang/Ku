/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "log_level.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

class Message : private util::noncopyable
{
public:
  Message() = default;
  Message(BufferQueue& free_queue) : buffer_(free_queue) { }
  Message(Message&& message)
    : log_level_(message.log_level_), buffer_(std::move(message.buffer_)) { }

  iovec const* raw_buffer() const { return buffer_.raw_buffer(); }
  size_t raw_buffer_count() const { return buffer_.raw_buffer_count(); }

  Buffer& buffer() { return buffer_; }

  void append(char const* str, size_t count) { buffer_.append(str, count); }
  void append(char c) { buffer_.append(c); }

  LogLevel log_level() { return log_level_; }
  void set_log_level(LogLevel log_level) { log_level_ = log_level; }

private:
  LogLevel log_level_;
  Buffer buffer_;
};

} } // namespace ku::log

