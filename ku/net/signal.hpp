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
struct Signal;
} // namespace ku::net::ops

// Wrapper for signalfd
class Signal
{
  typedef Handle<ops::Signal> HandleType;

public:
  Signal(); // TODO parameters

  HandleType const& handle() const { return handle_; }

  /*
  ssize_t read(uint64_t& val, size_t)
  { return ops::Common::read(handle_, &val, sizeof(val)); }

  ssize_t write(uint64_t val, size_t)
  { return ops::Common::write(handle_, &val, sizeof(val)); }
  */

private:
  HandleType handle_;
};

} } // namespace ku::net

