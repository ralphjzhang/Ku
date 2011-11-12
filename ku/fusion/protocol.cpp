/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cstring>
#include "protocol.hpp"

namespace ku { namespace fusion {

std::string to_str(Protocol p)
{
  switch (p) {
  case Protocol::inproc:
    return "inproc";
  case Protocol::ipc:
    return "ipc";
  case Protocol::tcp:
    return "tcp";
  case Protocol::pgm:
    return "pgm";
  case Protocol::ws:
    return "ws";
  default:
    return "invalid";
  }
}

Protocol from_str(std::string const& s)
{
  char const* protocols[] = {
    "inproc", "ipc", "tcp", "pgm", "ws"
  };
  for (unsigned i = 0; i < sizeof(protocols); ++i)
    if (s.compare(protocols[i]) == 0)
      return static_cast<Protocol>(i + 1);
  return Protocol::invalid;
}

} } // namespace ku::fusion

