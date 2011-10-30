/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <strings.h>
#include <cassert>
#include "util.hpp"
#include "timer_ops.hpp"
#include "timer.hpp"

namespace ku { namespace net {

Timer::Timer(Clock clock)
  : handle_(ops::Timer::create(static_cast<int>(clock))), mode_(None)
{
}

std::chrono::nanoseconds Timer::get_interval_internal()
{
  itimerspec spec;
  if (ops::Timer::get_time(handle_, spec))
    return util::from_timespec(spec.it_interval);
  return std::chrono::nanoseconds(0);
}

bool Timer::set_timespec(Mode mode, std::chrono::nanoseconds duration)
{
  itimerspec spec;
  ::bzero(&spec, sizeof(itimerspec));
  if (mode == Timer::Periodic) {
    spec.it_value = util::to_timespec(duration);
    spec.it_interval = util::to_timespec(duration);
  } else if (mode == Timer::Deadline) {
    spec.it_value = util::to_timespec(duration);
  } else {
    assert(false);
  }
  if (ops::Timer::set_time(handle_, spec))
    mode_ = mode;
  return !handle_.error();
}

bool Timer::clear()
{
  if (mode() == Timer::None)
    return false;
  else
    return set_timespec(mode(), std::chrono::nanoseconds(0));
}

} } // namespace ku::net

