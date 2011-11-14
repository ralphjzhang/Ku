/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/eventfd.h>
#include <system_error>
#include "util.hpp"
#include "handle.hpp"

namespace ku { namespace fusion { namespace ops {

struct UserEvent
{
  static inline Handle<UserEvent> create(int init_value, bool non_block)
  {
    int flag = non_block ? (EFD_NONBLOCK | EFD_CLOEXEC) : EFD_CLOEXEC;
    Handle<UserEvent> event_handle(::eventfd(init_value, flag));
    if (!event_handle)
      throw std::system_error(util::errc(), "ops::UserEvent::create");
    return std::move(event_handle);
  }
};

} } } // namespace ku::fusion::ops

