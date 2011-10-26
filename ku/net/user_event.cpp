#include <sys/eventfd.h>
#include "user_event.hpp"

namespace ku { namespace net {

UserEvent::UserEvent(unsigned init_value)
  : handle_(::eventfd(init_value, EFD_NONBLOCK | EFD_CLOEXEC), true)
{
  if (!handle_)
    handle_.set_error(errno);
}

} } // namespace ku::net

