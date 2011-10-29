/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/eventfd.h>
#include "handle.hpp"

namespace ku { namespace net { namespace ops {

struct UserEvent
{
  static inline Handle<UserEvent> create(int init_value, bool non_block = true)
  {
    int flag = non_block ? (EFD_NONBLOCK | EFD_CLOEXEC) : EFD_CLOEXEC;
    Handle<UserEvent> h(::eventfd(init_value, flag));
    if (!h)
      h.set_error(errno);
    return std::move(h);
  }
};

} } } // namespace ku::net::ops

