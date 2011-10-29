#include "user_event.hpp"
#include "user_event_ops.hpp"

namespace ku { namespace net {

UserEvent::UserEvent(unsigned init_value)
  : handle_(ops::UserEvent::create(init_value))
{
}

} } // namespace ku::net

