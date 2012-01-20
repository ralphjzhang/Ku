/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/epoll.h>
#include <chrono>
#include "event_buffer.hpp"

namespace ku { namespace fusion {

class Poller
{
public:
  Poller(int flags, size_t buf_size);
  void poll(std::chrono::milliseconds const& timeout);

private:
  EventBuffer event_buffer_;
  unsigned active_count_;
  int raw_handle_;
};

} } // namespace ku::fusion

