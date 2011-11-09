/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <chrono>
#include "message.hpp"
#include "logger.hpp"

namespace ku { namespace log {

Logger::Logger()
{
  // Initial free buffer space pool, ideally it can be WriteBlock * number-of-work-threads
  free_queue_.allocate_space(4096);
  message_queue_.reserve(16);
}

Logger::~Logger()
{
  flush();
}

void Logger::submit(Message&& message)
{
  bool write(false);
  {
    std::lock_guard<std::mutex> lock(message_queue_mutex_);
    message_queue_.emplace_back(std::move(message));
    write = message_queue_.size() >= 16;
  }
  if (write)
    write_condition_.notify_one();
}

void Logger::write()
{
  // TODO outer loop, quit machanism
  std::unique_lock<std::mutex> lock(message_queue_mutex_);
  while (message_queue_.empty())
    write_condition_.wait_for(lock, std::chrono::seconds(5));
  MessageQueue queue(std::move(message_queue_));
  lock.unlock();

  for (auto& sink_ptr : sink_list_) {
    ; // TODO write queue to sink
  }
  {
    std::lock_guard<std::mutex> lock(free_queue_mutex_);
    free_queue_.combine(std::move(queue.buffers()));
  }
}


void Logger::flush()
{
  //submit(write_queue_);
}

Logger& logger()
{
  // TODO this should be thread_local
  static /*thread_local*/ Logger lg;
  return lg;
}

} } // namespace ku::log

