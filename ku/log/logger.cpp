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

Logger::Logger() : quit_(false)
{
  // Initial free buffer space pool, a typical flush size of MessageQueue is 4K.
  // FreeHeap doubles this to allow free space allocated during flushing operation.
  const static size_t FreeHeap = 8192;
  free_queue_.allocate_space(FreeHeap);
  message_queue_.reserve();
  std::thread(&Logger::write, this).swap(thread_);
}

Logger::~Logger()
{
  quit_ = true;
  write_condition_.notify_one();
  thread_.join();
}

void Logger::submit(Message&& message)
{
  bool flushable(false);
  {
    std::lock_guard<std::mutex> lock(message_queue_mutex_);
    message_queue_.emplace_back(std::move(message));
    flushable = message_queue_.flushable();
  }
  if (flushable)
    write_condition_.notify_one();
}

void Logger::write()
{
  while (!quit_) {
    std::unique_lock<std::mutex> lock(message_queue_mutex_);
    while (message_queue_.empty() && !quit_)
      write_condition_.wait_for(lock, std::chrono::seconds(3));
    if (message_queue_.empty())
      continue;
    MessageQueue queue(std::move(message_queue_));
    lock.unlock();

    for (auto& sink_ptr : sink_list_)
      queue.flush_to(*sink_ptr);
    {
      // Message flushed, return heap space back to free_queue_
      std::lock_guard<std::mutex> lock(free_queue_mutex_);
      free_queue_.combine(std::move(queue.buffers()));
      // TODO might need a shrinking strategy
    }
  }
}

Logger& logger()
{
  // TODO: call_once
  static Logger lg;
  return lg;
}

} } // namespace ku::log

