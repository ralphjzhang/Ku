/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <string>
#include "protocol.hpp"

namespace ku { namespace fusion {

class Endpoint
{
public:
  Endpoint() : protocol_(Protocol::invalid) { }
  Endpoint(char const* uri);
  virtual ~Endpoint() { }

  Protocol protocol() const { return protocol_; }
  std::string const& address() const { return address_; }

private:
  bool parse(char const* str);

private:
  Protocol protocol_;
  std::string address_;
};

} } // namespace ku::fusion

