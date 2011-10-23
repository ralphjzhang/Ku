/*
 * Copyright 2011, Zhang, Jun. All rights reserved.
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com)
 *
 * This source code is provided with absolutely no warranty.
 */ 
#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <errno.h>
#include "address.hpp"
#include "raw_buffer.hpp"
#include "handle.hpp"

namespace ku { namespace net {

inline bool bind(Handle& h, Address const& addr)
{
  h.set_error(
      ::bind(h.raw_handle(), util::sockaddr_cast(&addr.sockaddr()), sizeof(sockaddr)) == -1 ? 
      errno : 0);         
  return !h.error();
}

inline bool listen(Handle& h)
{
  h.set_error(::listen(h.raw_handle(), SOMAXCONN) == -1 ? errno : 0);
  return !h.error();
}

inline bool set_non_block(Handle& h)
{
  int flags = ::fcntl(h.raw_handle(), F_GETFL, 0);
  h.set_error(flags == -1 ? errno : 0);
  if (!h.error())
    h.set_error(::fcntl(h.raw_handle(), F_SETFL, flags | O_NONBLOCK) == -1 ? errno : 0);
  return !h.error();
}

inline bool set_close_exec(Handle& h)
{
  int flags = ::fcntl(h.raw_handle(), F_GETFL, 0);
  h.set_error(flags == -1 ? errno : 0);
  if (!h.error())
    h.set_error(::fcntl(h.raw_handle(), F_SETFD, flags | FD_CLOEXEC) == -1 ? errno : 0);
  return !h.error();
}

inline Handle accept(Handle& h, Address& addr)
{
  socklen_t addr_len = sizeof(sockaddr_in);
  int conn_fd = ::accept4(h.raw_handle(), util::sockaddr_cast(&addr.sockaddr()), &addr_len,
      SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd == -1)
    return Handle(0, errno, true);
  else
    return Handle(conn_fd, true);
}


template <typename Buffer>
inline ssize_t read(Handle& h, Buffer&& buf, size_t count)
{
  ssize_t ret = ::read(h.raw_handle(), raw_buffer(buf), count);
  if (ret == -1) h.set_error(errno);
  return ret;
}

template <typename Buffer>
inline ssize_t write(Handle& h, Buffer& buf, size_t count)
{
  ssize_t ret = ::write(h.raw_handle(), raw_buffer(buf), count);
  if (ret == -1) h.set_error(errno);
  return ret;
}

/*
template <typename Buffer>
inline ssize_t writev(Handle& h, Buffer& buf, size_t count)
{
  // TODO
}*/

} } // namespace ku::net


