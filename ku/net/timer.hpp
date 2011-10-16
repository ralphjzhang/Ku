#pragma once
#include <system_error>
#include <chrono>
#include "handle.hpp"

namespace ku { namespace net {

/**
 * A Timer object owns a timer_fd file descriptor upon creation.
 * Ownership can be transferred.
 **/
class Timer : public Handle
{
  explicit Timer(int raw_handle) : Handle(raw_handle, true) { }

  template <typename Err>
  Timer(int raw_handle, Err err) : Handle(raw_handle, err, true) { }

public:
  enum Clock { Monotonic = CLOCK_MONOTONIC, Realtime = CLOCK_REALTIME };
  static Timer create(Clock clock = Clock::Monotonic);

  Timer(Timer&& t) : Handle(std::move(t)) { }
  ~Timer() = default;

  bool set_interval(std::chrono::nanoseconds interval);
  bool clear_interval() { return set_interval(std::chrono::nanoseconds(0)); }

  template <typename Duration = std::chrono::nanoseconds>
  Duration interval() { return std::chrono::duration_cast<Duration>(get_interval_()); }

private:
  std::chrono::nanoseconds get_interval_();
};

} } // namespace ku::net

