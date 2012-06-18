/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once

namespace ku { namespace fusion {

class Endpoint;

class UserEventEndpoint
{
  using Address = size_t;

public:
  UserEventEndpoint(Endpoint const& endpoint);
  ~UserEventEndpoint() = default;

  Address address() const { return address_; }

private:
  Address address_;
};

} } // namespace ku::fusion

