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
#include "../util.hpp"
#include "../ip_endpoint.hpp"
#include "../handle.hpp"

namespace ku { namespace fusion { namespace ops {

struct Socket
{
  static inline Handle<Socket> create(addrinfo const& ai, bool non_block = true)
  {
    addrinfo const* ai_ptr = &ai;
    while (ai_ptr) {
      int flag = ai_ptr->ai_socktype | SOCK_CLOEXEC;
      if (non_block)
        flag |= SOCK_NONBLOCK;
      Handle<Socket> socket_handle(::socket(ai_ptr->ai_family, flag, ai_ptr->ai_protocol));
      if (socket_handle)
        return std::move(socket_handle);
      ai_ptr = ai_ptr->ai_next;
    }
    throw std::system_error(util::errc(), "ops::Socket::create");
  }

  static inline void bind(Handle<Socket>& h, IPEndpoint const& endpoint)
  {
    if (::bind(h.raw_handle(), &endpoint.sockaddress(), endpoint.sockaddr_size()) == -1)
      throw std::system_error(util::errc(), "ops::Socket::bind");
  }

  static inline void listen(Handle<Socket>& h)
  {
    if (::listen(h.raw_handle(), SOMAXCONN) == -1)
      throw std::system_error(util::errc(), "ops::Socket::listen");
  }

  static inline Handle<Socket> accept(Handle<Socket>& h, IPEndpoint& endpoint, bool non_block = true)
  {
    int flag = non_block ? (SOCK_NONBLOCK | SOCK_CLOEXEC) : SOCK_CLOEXEC;
    sockaddr_storage addr;
    socklen_t addr_len = sizeof(sockaddr_storage);
    Handle<Socket> socket_handle(
        ::accept4(h.raw_handle(), reinterpret_cast<sockaddr*>(&addr), &addr_len, flag));
    if (!socket_handle && errno != EAGAIN && errno != EWOULDBLOCK)
      throw std::system_error(util::errc(), "ops::Socket::accept");
    // TODO think if we do this at all, client can use getpeername() to get it later, saving a lot of trouble
    if (addr_len == sizeof(sockaddr_in))
      endpoint = IPEndpoint(*reinterpret_cast<sockaddr_in*>(&addr));
    else if (addr_len == sizeof(sockaddr_in6))
      endpoint = IPEndpoint(*reinterpret_cast<sockaddr_in6*>(&addr));
    return socket_handle;
  }

  static inline void connect(Handle<Socket>& h, IPEndpoint const& endpoint)
  {
    if (::connect(h.raw_handle(), &endpoint.sockaddress(), endpoint.sockaddr_size()) == -1)
      throw std::system_error(util::errc(), "ops::Socket::connect");
  }

private:
  
};

} } } // namespace ku::fusion::ops


