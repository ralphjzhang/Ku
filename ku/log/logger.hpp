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
#include "log_level.hpp"
#include "buffer_list.hpp"
#include "message_queue.hpp"
#include "sink.hpp"
#include "collector.hpp"

namespace ku { namespace log {

class Message;

class Logger
{
  using SinkList = std::forward_list<Sink_ptr>;
  friend Logger& g_logger();

public:
  ~Logger();
  void add_sink(Sink_ptr sink) { sink_list_.push_front(std::move(sink)); }

  Collector create_collector(LogLevel log_level)
  {
    std::lock_guard<std::mutex> lock(free_queue_mutex_);
    return Collector(log_level, free_queue_, *this);
  }

  void submit(Message&& message);
  LogLevel log_level() const { return log_level_; }
  void set_log_level(LogLevel log_level) { log_level_ = log_level; }

private:
  Logger();

  void write();

private:
  std::thread thread_;
  MessageQueue message_queue_;
  BufferList free_queue_;
  std::mutex message_queue_mutex_, free_queue_mutex_;
  std::condition_variable write_condition_;
  SinkList sink_list_;
  bool quit_;
  LogLevel log_level_;
};

Logger& g_logger();

} } // namespace ku::log

