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
#include <errno.h>
#include <netdb.h>
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
    Handle<Socket> h(::socket(ai.ai_family, flag, ai.ai_protocol), true);
    if (!h)
      h.set_error(errno);
    return std::move(h);
  }

  static inline bool bind(Handle<Socket>& h, Endpoint const& endpoint)
  {
    h.set_error(::bind(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()),
          sizeof(sockaddr)) == -1 ?  errno : 0);         
    return !h.error();
  }

  static inline bool listen(Handle<Socket>& h)
  {
    h.set_error(::listen(h.raw_handle(), SOMAXCONN) == -1 ? errno : 0);
    return !h.error();
  }

  static inline Handle<Socket> accept(Handle<Socket>& h, Endpoint& endpoint, bool non_block = true)
  {
    int flag = non_block ? (SOCK_NONBLOCK | SOCK_CLOEXEC) : SOCK_CLOEXEC;
    socklen_t addr_len = sizeof(sockaddr_in);
    Handle<Socket> socket_handle(
        ::accept4(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()), &addr_len, flag), true);
    h.set_error(socket_handle ? 0 : errno);
    return socket_handle;
  }

  static inline bool connect(Handle<Socket>& h, Endpoint const& endpoint)
  {
    h.set_error(::connect(h.raw_handle(), util::sockaddr_cast(&endpoint.sockaddr()),
          sizeof(sockaddr)) == -1 ? errno : 0);
    return !h.error();
  }
};

} } } // namespace ku::net::ops


