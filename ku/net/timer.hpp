#pragma once
#include <system_error>
#include <chrono>
#include "handle.hpp"

namespace ku { namespace net {

/**
 * A Timer object owns a timer_fd file descriptor upon creation.
 * Ownership can be transferred.
 **/
class Timer
{
public:
  enum Clock { Monotonic = CLOCK_MONOTONIC, Realtime = CLOCK_REALTIME };
  Timer(Clock clock = Clock::Monotonic);

  Timer(Timer&& t) : handle_(std::move(t.release_handle())) { }
  ~Timer() = default;

  bool set_interval(std::chrono::nanoseconds interval);
  bool clear_interval() { return set_interval(std::chrono::nanoseconds(0)); }

  template <typename Duration = std::chrono::nanoseconds>
  Duration interval() { return std::chrono::duration_cast<Duration>(get_interval_()); }

  Handle release_handle() { return std::move(handle_); }
  std::error_code error() const { return handle_.error(); }

private:
  std::chrono::nanoseconds get_interval_();
  Handle handle_;
};

} } // namespace ku::net

