/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <forward_list>
#include <atomic>
#include <mutex>
#include <thread>
#include "sink.hpp"
#include "util.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

class Buffer;
class Logger;

class Writer : private util::noncopyable
{
  typedef std::forward_list<Sink_ptr> SinkList;

public:
  Writer(Logger& logger) : logger_(logger) { }
  ~Writer() { thread_.join(); }

  void start();
  std::unique_lock<std::mutex> aquire_lock() { return std::unique_lock<std::mutex>(mutex_); }

  void add_sink(Sink_ptr sink) { sink_list_.push_front(std::move(sink)); }
  void operator () ();
  void quit() { quit_ = true; }

  void submit(Buffer& buf);

private:
  Logger& logger_;
  std::atomic<bool> quit_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::thread thread_;
  Buffer buffer_;
  SinkList sink_list_;
};

} } // namespace ku::log


