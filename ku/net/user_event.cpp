/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "user_event.hpp"
#include "user_event_ops.hpp"

namespace ku { namespace net {

UserEvent::UserEvent(unsigned init_value)
  : handle_(ops::UserEvent::create(init_value))
{
}

} } // namespace ku::net

