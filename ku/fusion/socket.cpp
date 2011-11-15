/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include "util.hpp"
#include "ops/socket.hpp"
#include "socket_endpoint.hpp"
#include "socket.hpp"

namespace {

using ku::fusion::SocketEndpoint;

inline unsigned short sa_family(SocketEndpoint::AddressFamily af)
{
  return af == SocketEndpoint::Unix ? AF_UNIX
             : SocketEndpoint::IPv4 ? AF_INET
             : SocketEndpoint::IPv6 ? AF_INET6 : 0;
}

// TODO ai_protocol needs a change to support udp/datagrams
addrinfo acceptor_addrinfo(SocketEndpoint const& endpoint)
{
  addrinfo aif;
  std::memset(&aif, 0, sizeof(addrinfo));
  aif.ai_family = sa_family(endpoint.address_family());
  aif.ai_protocol = endpoint.address_family() == SocketEndpoint::Unix ? 0 : IPPROTO_TCP;
  aif.ai_socktype = SOCK_STREAM;
  aif.ai_flags = AI_PASSIVE;
  return aif;
}

addrinfo connector_addrinfo(SocketEndpoint const& endpoint)
{
  addrinfo aif;
  std::memset(&aif, 0, sizeof(addrinfo));
  aif.ai_family = sa_family(endpoint.address_family());
  aif.ai_protocol = endpoint.address_family() == SocketEndpoint::Unix ? 0 : IPPROTO_TCP;
  aif.ai_socktype = SOCK_STREAM;
  return aif;
}

} // unnamed namespace

namespace ku { namespace fusion {

/// Socket ///


/// AcceptorSocket ///

Socket AcceptorSocket::accept(SocketEndpoint& endpoint)
{
  return Socket(ops::Socket::accept(handle_, endpoint));
}

void AcceptorSocket::bind_listen(SocketEndpoint const& endpoint)
{
  handle_.close();
  handle_ = ops::Socket::create(acceptor_addrinfo(endpoint));
  if (endpoint.address_family() == SocketEndpoint::Unix)
    if (::unlink(endpoint.address().c_str()) == -1)
      throw std::system_error(util::errc(), "AcceptorSocket::bind_listen");
  ops::Socket::bind(handle_, endpoint);
  ops::Socket::listen(handle_);
}

/// ConnectorSocket ///

void ConnectorSocket::connect(SocketEndpoint const& endpoint, bool non_block)
{
  handle_.close();
  handle_ = ops::Socket::create(connector_addrinfo(endpoint), non_block);
  ops::Socket::connect(handle_, endpoint);
}

} } // namespace ku::fusion


