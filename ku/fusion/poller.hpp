/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/epoll.h>
#include <chrono>
#include <atomic>
#include "event_buffer.hpp"

namespace ku { namespace fusion {

class Poller
{
public:
  Poller(int flags, size_t buf_size);

  // TODO quit should be able to wake up poll loop (probably with an eventfd)
  void quit_loop() { quit_.store(true, std::memory_order_release); }
  void loop(std::chrono::milliseconds const& timeout);
  void operator()(std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  bool add_event(int raw_fd, epoll_event* event);
  bool remove_event(int raw_fd);
  bool modify_event(int raw_fd, epoll_event* event);

private:
  void poll(std::chrono::milliseconds const& timeout);

private:
  EventBuffer event_buffer_;
  unsigned active_count_;
  int raw_handle_;
  std::atomic_bool quit_;
};

} } // namespace ku::fusion

