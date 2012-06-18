/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <algorithm>
#include <vector>
#include "buffer_list.hpp"
#include "message.hpp"

namespace ku { namespace log {

class Sink;

class MessageQueue
{
  struct MessageInfo
  {
    MessageInfo(LogLevel level, uint32_t count) : log_level(level), raw_buffer_count(count) { }
    LogLevel log_level;
    uint32_t raw_buffer_count;
  };

  using BufferIndex = std::vector<MessageInfo>;
  const static size_t FlushCount = 16;

public:
  MessageQueue() : min_log_level_(LogLevel::Fatal) { }
  // move constructor is NOT thread safe, lock it when use
  MessageQueue(MessageQueue&& queue)
    : index_(std::move(queue.index_)) , buffers_(std::move(queue.buffers_))
    , min_log_level_(LogLevel::Fatal)
  { }

  void emplace_back(Message&& message)
  {
    index_.emplace_back(message.log_level(), message.raw_buffer_count());
    buffers_.emplace_back(std::move(message.buffer()));
    min_log_level_ = std::min(min_log_level_, message.log_level());
  }

  void reserve() { index_.reserve(FlushCount); buffers_.reserve(FlushCount + FlushCount / 2); }
  inline bool empty() { return index_.empty(); }

  bool flushable() const { return buffers_.raw_buffer_count() >= FlushCount; }
  void flush_to(Sink& sink);

  BufferList& buffers() { return buffers_; }

private:
  BufferIndex index_;
  BufferList buffers_;
  LogLevel min_log_level_;
};

} } // namespace ku::log

