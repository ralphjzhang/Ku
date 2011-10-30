/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/signalfd.h>
#include <signal.h>
#include <initializer_list>
#include "handle.hpp"

namespace ku { namespace net { namespace ops {

struct Signal
{
  static inline Handle<Signal> create(std::initializer_list<int> signals, bool non_block = true)
  {
    int flag = non_block ? (SFD_NONBLOCK | SFD_CLOEXEC) : SFD_NONBLOCK;
    sigset_t mask;
    ::sigemptyset(&mask);
    for (int signal : signals)
      ::sigaddset(&mask, signal);
    Handle<Signal> h(::signalfd(-1, &mask, flag));
    if (!h)
      h.set_error(errno);
    return std::move(h);
  }
};


} } } // namespace ku::net::ops

