/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <netdb.h>
#include <system_error>
#include "util.hpp"
#include "endpoint.hpp"
#include "handle.hpp"

namespace ku { namespace net { namespace ops {

struct Socket
{
  static inline Handle<Socket> create(addrinfo const& ai, bool non_block = true)
  {
    int flag = ai.ai_socktype | SOCK_CLOEXEC;
    if (non_block)
      flag |= SOCK_NONBLOCK;
    Handle<Socket> socket_handle(::socket(ai.ai_family, flag, ai.ai_protocol));
    if (!socket_handle)
      throw std::system_error(util::errc(), "ops::Socket::create");
    else
      return std::move(socket_handle);
  }

  static inline void bind(Handle<Socket>& h, Endpoint const& endpoint)
  {
    if (::bind(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()), sizeof(sockaddr)) == -1)
      throw std::system_error(util::errc(), "ops::Socket::bind");
  }

  static inline void listen(Handle<Socket>& h)
  {
    if (::listen(h.raw_handle(), SOMAXCONN) == -1)
      throw std::system_error(util::errc(), "ops::Socket::listen");
  }

  static inline Handle<Socket> accept(Handle<Socket>& h, Endpoint& endpoint, bool non_block = true)
  {
    int flag = non_block ? (SOCK_NONBLOCK | SOCK_CLOEXEC) : SOCK_CLOEXEC;
    socklen_t addr_len = sizeof(sockaddr_in);
    Handle<Socket> socket_handle(
        ::accept4(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()), &addr_len, flag));
    if (!socket_handle && errno != EAGAIN && errno != EWOULDBLOCK)
      throw std::system_error(util::errc(), "ops::Socket::accept");
    return socket_handle;
  }

  static inline void connect(Handle<Socket>& h, Endpoint const& endpoint)
  {
    if (::connect(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()), sizeof(sockaddr)) == -1)
      throw std::system_error(util::errc(), "ops::Socket::connect");
  }
};

} } } // namespace ku::net::ops


