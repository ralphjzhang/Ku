#pragma once
#include "handle.hpp"

namespace ku { namespace net {

// Wrapper for eventfd
class UserEvent
{
public:
  UserEvent(unsigned init_value);

private:
  Handle handle_;
};

} } // namespace ku::net

