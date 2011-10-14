#pragma once
#include <system_error>
#include <chrono>
#include "util.hpp"

namespace ku { namespace net {

class Timer : private util::noncopyable
{
  explicit Timer(int raw_handle) : raw_handle_(raw_handle) { }

  template <typename Err>
  Timer(int raw_handle, Err err) : raw_handle_(raw_handle) { set_error(err); }

public:
  enum Clock { Monotonic = CLOCK_MONOTONIC, Realtime = CLOCK_REALTIME };

  Timer(Timer&& t);
  ~Timer() { close(); }

  static Timer create(Clock clock = Clock::Monotonic);
  int raw_handle() const { return raw_handle_; }
  int release_handle();

  bool set_interval(std::chrono::nanoseconds interval);

  template <typename Duration = std::chrono::nanoseconds>
  Duration interval() { return std::chrono::duration_cast<Duration>(get_interval_()); }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }
  void close();

private:
  std::chrono::nanoseconds get_interval_();

  int raw_handle_;
  std::error_code error_;
};

} } // namespace ku::net

