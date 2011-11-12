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

namespace ku { namespace fusion {

Endpoint::Endpoint(char const* uri)
{
  if (!parse(uri))
    throw std::system_error(util::errc(EINVAL), "Endpoint::Endpoint");
}

bool Endpoint::parse(char const* str)
{
  assert(str);
  std::string uri(str);
  std::string::size_type pos = uri.find ("://");
  if (pos == std::string::npos)
    return false;
  protocol_ = str_to_protocol(uri.substr(0, pos));
  address_ = uri.substr(pos + 3);
  if (!protocol_ || address_.empty())
    return false;
  return true;
}

} } // namespace ku::fusion


