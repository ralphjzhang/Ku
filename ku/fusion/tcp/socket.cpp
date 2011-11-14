/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <strings.h>
#include <netdb.h>
#include <cerrno>
#include "../util.hpp"
#include "../ops/socket.hpp"
#include "../ip_endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion { namespace tcp {

addrinfo acceptor_addrinfo()
{
  addrinfo aif;
  ::bzero(&aif, sizeof(addrinfo));
  aif.ai_family = AF_INET;
  aif.ai_protocol = IPPROTO_TCP;
  aif.ai_socktype = SOCK_STREAM;
  aif.ai_flags = AI_PASSIVE;
  return aif;
}

/// Socket ///


/// AcceptorSocket ///

AcceptorSocket::AcceptorSocket(IPEndpoint const& endpoint)
  : handle_(ops::Socket::create(acceptor_addrinfo()))
{
  // TODO maybe let user do this ?
  if (handle_) {
    bind(endpoint);
    listen();
  }
}

Socket AcceptorSocket::accept(IPEndpoint& endpoint)
{
  return Socket(ops::Socket::accept(handle_, endpoint));
}

void AcceptorSocket::bind(IPEndpoint const& endpoint)
{
  ops::Socket::bind(handle_, endpoint);
}

void AcceptorSocket::listen()
{
  ops::Socket::listen(handle_);
}

/// ConnectorSocket ///

ConnectorSocket::ConnectorSocket(bool non_block)
  : Socket(ops::Socket::create(acceptor_addrinfo(), non_block)) // TODO remove AI_PASSIVE
{
}

void ConnectorSocket::connect(IPEndpoint const& endpoint)
{
  ops::Socket::connect(handle_, endpoint);
}

} } } // namespace ku::fusion::tcp


