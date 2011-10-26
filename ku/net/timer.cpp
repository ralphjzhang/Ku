#include <strings.h>
#include <sys/timerfd.h>
#include <cassert>
#include "util.hpp"
#include "timer.hpp"

namespace ku { namespace net {

Timer::Timer(Clock clock)
  : handle_(::timerfd_create(static_cast<int>(clock), TFD_NONBLOCK | TFD_CLOEXEC), true), mode_(None)
{
  if (!handle_)
    handle_.set_error(errno);
}

std::chrono::nanoseconds Timer::get_interval_internal()
{
  itimerspec spec;
  if (::timerfd_gettime(handle_.raw_handle(), &spec) != -1)
    return util::from_timespec(spec.it_interval);
  handle_.set_error(errno);
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
  if (::timerfd_settime(handle_.raw_handle(), 0, &spec, NULL) != -1) {
    mode_ = mode;
    return true;
  }
  handle_.set_error(errno);
  return false;
}

} } // namespace ku::net

