/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cstdlib>
#include <string>
#include <system_error>
#include "util.hpp"
#include "endpoint.hpp"
#include "user_event_endpoint.hpp"

namespace ku { namespace fusion {

UserEventEndpoint::UserEventEndpoint(Endpoint const& endpoint)
{
  std::string const& ep = endpoint.address();
  if (ep[0] == '#')
    address_ = std::atoll(ep.substr(1).c_str());
  if (!address_)
    throw std::system_error(util::errc(EINVAL), "UserEventEndpoint::UserEventEndpoint");
}

} } // namespace ku::fusion

