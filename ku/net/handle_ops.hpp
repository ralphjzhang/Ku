/*
 * Copyright 2011, Zhang, Jun. All rights reserved.
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com)
 *
 * This source code is provided with absolutely no warranty.
 */ 
#pragma once
#include <unistd.h>
#include <sys/uio.h>
#include <errno.h>
#include "raw_buffer.hpp"
#include "handle.hpp"

namespace ku { namespace net {

class Address;

template <typename Handle, typename Buffer>
inline ssize_t read(Handle& h, Buffer&& buf, size_t count)
{
  ssize_t ret = ::read(h.raw_handle(), raw_buffer(buf), count);
  if (ret == -1) h.set_error(errno);
  return ret;
}

template <typename Handle, typename Buffer>
inline ssize_t write(Handle& h, Buffer& buf, size_t count)
{
  ssize_t ret = ::write(h.raw_handle(), raw_buffer(buf), count);
  if (ret == -1) h.set_error(errno);
  return ret;
}

template <typename Handle, typename Buffer>
inline ssize_t writev(Handle& h, Buffer& buf, size_t count)
{
  // TODO
}

} } // namespace ku::net


