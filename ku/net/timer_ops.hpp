/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/timerfd.h>
#include "handle.hpp"

namespace ku { namespace net { namespace ops {

struct Timer
{
  static inline Handle<Timer> create(int clock, bool non_block = true)
  {
    int flag = non_block ? (TFD_NONBLOCK | TFD_CLOEXEC) : TFD_CLOEXEC;
    Handle<Timer> h(::timerfd_create(clock, flag));
    if (!h)
      h.set_error(errno);
    return std::move(h);
  }

  static inline bool get_time(Handle<Timer>& h, itimerspec &spec)
  {
    h.set_error(::timerfd_gettime(h.raw_handle(), &spec) == -1 ? errno : 0);
    return !h.error();
  }

  static inline bool set_time(Handle<Timer>& h, itimerspec const& spec)
  {
    h.set_error(::timerfd_settime(h.raw_handle(), 0, &spec, NULL) == -1 ? errno : 0);
    return !h.error();
  }
};

} } } // namespace ku::net::ops

