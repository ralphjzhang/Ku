/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/timerfd.h>
#include <system_error>
#include "../util.hpp"
#include "../handle.hpp"

namespace ku { namespace fusion { namespace ops {

struct Timer
{
  static inline Handle<Timer> create(int clock, bool non_block = true)
  {
    int flag = non_block ? (TFD_NONBLOCK | TFD_CLOEXEC) : TFD_CLOEXEC;
    Handle<Timer> timer_handle(::timerfd_create(clock, flag));
    if (!timer_handle)
      throw std::system_error(util::errc(), "ops::Timer::create");
    return std::move(timer_handle);
  }

  static inline void get_time(Handle<Timer>& h, itimerspec &spec)
  {
    if (::timerfd_gettime(h.raw_handle(), &spec) == -1)
      throw std::system_error(util::errc(), "ops::Timer::get_time");
  }

  static inline void set_time(Handle<Timer>& h, itimerspec const& spec)
  {
    if (::timerfd_settime(h.raw_handle(), 0, &spec, NULL) == -1)
      throw std::system_error(util::errc(), "ops::Timer::set_time");
  }
};

} } } // namespace ku::fusion::ops

