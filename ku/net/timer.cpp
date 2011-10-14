#include <strings.h>
#include <sys/timerfd.h>
#include "sys.hpp"
#include "timer.hpp"

namespace ku { namespace net {

Timer::Timer(Timer&& t)
  : raw_handle_(t.raw_handle_), error_(t.error_)
{
  t.clear();
}

Timer Timer::create(Clock clock)
{
  int timer_fd = ::timerfd_create(static_cast<int>(clock), TFD_NONBLOCK | TFD_CLOEXEC);
  return timer_fd == -1 ? Timer(timer_fd, errno) : Timer(timer_fd);
}

int Timer::release_handle()
{
  int handle = raw_handle_;
  raw_handle_ = 0;
  return handle;
}

bool Timer::set_interval(std::chrono::nanoseconds ns)
{
  itimerspec spec;
  ::bzero(&spec, sizeof(itimerspec));
  spec.it_interval = sys::to_timespec(ns);
  if (::timerfd_settime(raw_handle_, 0, &spec, NULL) != -1)
    return true;
  set_error(errno);
  return false;
}

void Timer::close()
{
  if (raw_handle_) {
    if (::close(raw_handle_) == -1)
      set_error(errno);
    else
      clear();
  }
}

std::chrono::nanoseconds Timer::get_interval_()
{
  itimerspec spec;
  if (::timerfd_gettime(raw_handle_, &spec) != -1)
    return sys::from_timespec(spec.it_interval);
  set_error(errno);
  return std::chrono::nanoseconds(0);
}

} } // namespace ku::net

