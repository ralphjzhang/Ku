/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <system_error>
#include "util.hpp"
#include "endpoint.hpp"

namespace {

using ku::fusion::Protocol;

Protocol parse_uri(char const* str, std::string& address)
{
  assert(str);
  std::string uri(str);
  std::string::size_type pos = uri.find ("://");
  if (pos == std::string::npos)
    return Protocol::Invalid;
  address = uri.substr(pos + 3);
  return ku::fusion::str_to_protocol(uri.substr(0, pos));
}

} // unamed namespace

namespace ku { namespace fusion {

bool Endpoint::resolve(char const* uri)
{
  protocol_ = parse_uri(uri, address_);
  return protocol_ != Protocol::Invalid;
}


} } // namespace ku::fusion


