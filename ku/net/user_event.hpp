/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "handle.hpp"

namespace ku { namespace net {

namespace ops {
struct UserEvent;
} // namespace ku::net::ops

// Wrapper for eventfd
class UserEvent
{
  typedef Handle<ops::UserEvent> HandleType;

public:
  UserEvent(unsigned init_value);

  HandleType const& handle() const { return handle_; }

private:
  HandleType handle_;
};

} } // namespace ku::net

