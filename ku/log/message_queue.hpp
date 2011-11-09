/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <vector>
#include "buffer_queue.hpp"
#include "message.hpp"

namespace ku { namespace log {

class MessageQueue
{
  struct MessageInfo
  {
    MessageInfo(LogLevel level, uint32_t count) : log_level(level), raw_buffer_count(count) { }
    LogLevel log_level;
    uint32_t raw_buffer_count;
  };

  typedef std::vector<MessageInfo> BufferIndex;

public:
  MessageQueue() = default;
  MessageQueue(MessageQueue&& queue) : index_(std::move(queue.index_)), buffers_(std::move(queue.buffers_)) { }

  void emplace_back(Message&& message)
  {
    index_.emplace_back(message.log_level(), message.raw_buffer_count());
    buffers_.emplace_back(std::move(message.buffer()));
  }

  BufferQueue& buffers() { return buffers_; }

private:
  BufferIndex index_;
  BufferQueue buffers_;
};

} } // namespace ku::log

