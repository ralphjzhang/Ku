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
  Endpoint() : protocol_(Protocol::Invalid) { }
  Endpoint(Endpoint const&) = default;

  Endpoint(Protocol p, std::string const& addr) : protocol_(p), address_(addr) { }
  Endpoint(Endpoint&& endpoint) : protocol_(endpoint.protocol_), address_(std::move(endpoint.address_)) { }
  virtual ~Endpoint() { }

  Protocol protocol() const { return protocol_; }
  std::string const& address() const { return address_; }

private:
  Protocol protocol_;
  std::string address_;
};

Endpoint resolve(char const* uri);

} } // namespace ku::fusion

