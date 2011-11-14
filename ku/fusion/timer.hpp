/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include <chrono>
#include <string>
#include "handle.hpp"
#include "ops/common.hpp"

namespace ku { namespace fusion {

namespace ops {
struct Timer;
} // namespace ku::fusion::ops

class Timer
{
  typedef Handle<ops::Timer> HandleType;

public:
  enum Clock { Monotonic = CLOCK_MONOTONIC, Realtime = CLOCK_REALTIME };
  enum Mode { None, Periodic, Deadline };

  Timer(Clock clock = Clock::Monotonic);

  Timer(Timer&& t) : handle_(std::move(t.handle_)), mode_(t.mode_) { }
  ~Timer() = default;

  HandleType const& handle() const { return handle_; }
  Mode mode() const { return mode_; }

  ssize_t read(uint64_t& val, size_t)
  { return ops::Common::read(handle_, &val, sizeof(val)); }

  ssize_t write(uint64_t val, size_t)
  { return ops::Common::write(handle_, &val, sizeof(val)); }

  void set_interval(std::chrono::nanoseconds interval) { set_timespec(Periodic, interval); }
  void clear();

  template <typename Duration = std::chrono::nanoseconds>
  Duration interval() { return std::chrono::duration_cast<Duration>(get_interval_internal()); }

  void set_expires_in(std::chrono::nanoseconds duration) { set_timespec(Deadline, duration); }

  template <typename Clock>
  void set_expires_at(std::chrono::time_point<Clock> expiry)
  { set_expires_in(expiry - Clock::now()); }

private:
  std::chrono::nanoseconds get_interval_internal();
  void set_timespec(Mode mode, std::chrono::nanoseconds duration);

  HandleType handle_;
  Mode mode_;
};

inline std::string to_str(Timer::Mode mode)
{
  char const* str[] = { "None", "Periodic", "Deadline" };
  return std::string(str[mode]);
}

} } // namespace ku::fusion

