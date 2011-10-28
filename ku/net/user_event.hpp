#pragma once
#include "handle.hpp"

namespace ku { namespace net {

// Wrapper for eventfd
class UserEvent
{
public:
  UserEvent(unsigned init_value);
  int raw_handle() const { return handle_.raw_handle(); }

private:
  Handle handle_;
};

} } // namespace ku::net

