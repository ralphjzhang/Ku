/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "user_event.hpp"
#include "ops/user_event.hpp"

namespace ku { namespace fusion {

UserEvent::UserEvent(unsigned init_value, bool non_block)
  : handle_(ops::UserEvent::create(init_value, non_block))
{
}

} } // namespace ku::fusion

