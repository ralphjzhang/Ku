/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <forward_list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "buffer_queue.hpp"
#include "message_queue.hpp"
#include "sink.hpp"
#include "collector.hpp"

namespace ku { namespace log {

class Message;

class Logger
{
  typedef std::forward_list<Sink_ptr> SinkList;

public:
  Logger();
  ~Logger();

  void add_sink(Sink_ptr sink) { sink_list_.push_front(std::move(sink)); }

  Collector create_collector()
  {
    std::lock_guard<std::mutex> lock(free_queue_mutex_);
    return Collector(free_queue_, *this);
  }

  void submit(Message&& message);

private:
  void write();
  void flush();

private:
  const static size_t WriteBlock = 1024;//4096;

  MessageQueue message_queue_;
  BufferQueue free_queue_;
  std::mutex message_queue_mutex_, free_queue_mutex_;
  std::condition_variable write_condition_;
  SinkList sink_list_;
};

Logger& logger();

} } // namespace ku::log

