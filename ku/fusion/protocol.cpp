/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <cstring>
#include "protocol.hpp"

namespace {
static char const* protocols[] = {
  "invalid", "inproc", "ipc", "tcp", "pgm", "ws"
};
} // unamed namespace

namespace ku { namespace fusion {

std::string to_str(Protocol p)
{
  assert(p >= Protocol::Invalid && p <= Protocol::WS);
  return protocols[static_cast<int>(p)];
}

Protocol str_to_protocol(std::string const& s)
{
  for (unsigned i = 0; i < sizeof(protocols); ++i)
    if (s.compare(protocols[i]) == 0)
      return static_cast<Protocol>(i + 1);
  return Protocol::Invalid;
}

} } // namespace ku::fusion

