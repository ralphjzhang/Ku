#include <strings.h>
#include <sys/timerfd.h>
#include "util.hpp"
#include "timer.hpp"

namespace ku { namespace net {

Timer::Timer(Clock clock)
  : handle_(::timerfd_create(static_cast<int>(clock), TFD_NONBLOCK | TFD_CLOEXEC), true)
{
  if (!handle_.raw_handle())
    handle_.set_error(errno);
}

bool Timer::set_interval(std::chrono::nanoseconds ns)
{
  itimerspec spec;
  ::bzero(&spec, sizeof(itimerspec));
  spec.it_value = util::to_timespec(ns);
  spec.it_interval = util::to_timespec(ns);
  if (::timerfd_settime(handle_.raw_handle(), 0, &spec, NULL) != -1)
    return true;
  handle_.set_error(errno);
  return false;
}

std::chrono::nanoseconds Timer::get_interval_()
{
  itimerspec spec;
  if (::timerfd_gettime(handle_.raw_handle(), &spec) != -1)
    return util::from_timespec(spec.it_interval);
  handle_.set_error(errno);
  return std::chrono::nanoseconds(0);
}

} } // namespace ku::net

