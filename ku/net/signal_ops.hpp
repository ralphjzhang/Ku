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
#include <system_error>
#include "util.hpp"
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
    Handle<Signal> signal_handle(::signalfd(-1, &mask, flag));
    if (!signal_handle)
      throw std::system_error(util::errc(), "ops::Signal::create");
    return std::move(signal_handle);
  }
};


} } } // namespace ku::net::ops

