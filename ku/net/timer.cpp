#include <strings.h>
#include <sys/timerfd.h>
#include "sys.hpp"
#include "timer.hpp"

namespace ku { namespace net {

Timer Timer::create(Clock clock)
{
  int timer_fd = ::timerfd_create(static_cast<int>(clock), TFD_NONBLOCK | TFD_CLOEXEC);
  return timer_fd == -1 ? Timer(timer_fd, errno) : Timer(timer_fd);
}

bool Timer::set_interval(std::chrono::nanoseconds ns)
{
  itimerspec spec;
  ::bzero(&spec, sizeof(itimerspec));
  spec.it_value = sys::to_timespec(ns);
  spec.it_interval = sys::to_timespec(ns);
  if (::timerfd_settime(raw_handle(), 0, &spec, NULL) != -1)
    return true;
  set_error(errno);
  return false;
}

std::chrono::nanoseconds Timer::get_interval_()
{
  itimerspec spec;
  if (::timerfd_gettime(raw_handle(), &spec) != -1)
    return sys::from_timespec(spec.it_interval);
  set_error(errno);
  return std::chrono::nanoseconds(0);
}

} } // namespace ku::net

