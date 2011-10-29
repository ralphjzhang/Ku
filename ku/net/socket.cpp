/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "endpoint.hpp"
#include "resolver.hpp"
#include "util.hpp"
#include "socket_ops.hpp"
#include "socket.hpp"

namespace ku { namespace net {

/// StreamSocket ///


/// AcceptorSocket ///

AcceptorSocket::AcceptorSocket(Endpoint const& endpoint)
  : handle_(ops::Socket::create(Resolver(Resolver::Passive)))
{
  // TODO maybe let user do this ?
  if (handle_)
    if (bind(endpoint))
      listen();
}

StreamSocket AcceptorSocket::accept(Endpoint& endpoint)
{
  return StreamSocket(ops::Socket::accept(handle_, endpoint));
}

bool AcceptorSocket::bind(Endpoint const& endpoint)
{
  return ops::Socket::bind(handle_, endpoint);
}

bool AcceptorSocket::listen()
{
  return ops::Socket::listen(handle_);
}

/// ConnectorSocket ///

ConnectorSocket::ConnectorSocket(bool non_block)
  : StreamSocket(ops::Socket::create(Resolver(), non_block))
{
}

bool ConnectorSocket::connect(Endpoint const& endpoint)
{
  return ops::Socket::connect(handle_, endpoint);
}

} } // namespace ku::net


