#pragma once
#include <system_error>
#include <chrono>
#include <string>
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
  enum Mode { None, Periodic, Deadline };

  Timer(Clock clock = Clock::Monotonic);

  Timer(Timer&& t) : handle_(std::move(t.release_handle())) { }
  ~Timer() = default;

  int raw_handle() const { return handle_.raw_handle(); }
  Mode mode() const { return mode_; }

  bool set_interval(std::chrono::nanoseconds interval) { return set_timespec(Periodic, interval); }
  bool clear_interval() { return set_interval(std::chrono::nanoseconds(0)); }

  template <typename Duration = std::chrono::nanoseconds>
  Duration interval() { return std::chrono::duration_cast<Duration>(get_interval_internal()); }

  template <typename Clock>
  bool set_expires_at(std::chrono::time_point<Clock> expiry)
  {
    std::error_code ec;
    auto duration = expiry - Clock::now(ec);
    if (!ec)
      return set_timespec(Deadline, duration);
    handle_.set_error(ec.value());
    return false;
  }

  bool set_expires_in(std::chrono::nanoseconds duration) { return set_timespec(Deadline, duration); }

  Handle release_handle() { return std::move(handle_); }
  std::error_code error() const { return handle_.error(); }

private:
  std::chrono::nanoseconds get_interval_internal();
  bool set_timespec(Mode mode, std::chrono::nanoseconds duration);

  Handle handle_;
  Mode mode_;
};

inline std::string to_str(Timer::Mode mode)
{
  char const* str[] = { "None", "Periodic", "Deadline" };
  return std::string(str[mode]);
}

} } // namespace ku::net

