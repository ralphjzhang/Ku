/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <string>

namespace ku { namespace fusion {

enum class Protocol
{
  invalid = 0, inproc, ipc, tcp, pgm, ws
};

inline bool operator!(Protocol p) { return static_cast<int>(p) > 0; }

std::string to_str(Protocol p);

Protocol str_to_protocol(std::string const& s);

} } // namespace ku::fusion

