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
#include "raw_buffer.hpp"
#include "handle.hpp"

namespace ku { namespace net { namespace ops {

struct Common
{
  template <typename T, typename Buffer>
  static inline ssize_t read(Handle<T>& h, Buffer&& buf, size_t count)
  {
    ssize_t ret = ::read(h.raw_handle(), raw_buffer(buf), count);
    if (ret == -1) h.set_error(errno);
    return ret;
  }

  template <typename T, typename Buffer>
  static inline ssize_t write(Handle<T>& h, Buffer const& buf, size_t count)
  {
    ssize_t ret = ::write(h.raw_handle(), raw_buffer(buf), count);
    if (ret == -1) h.set_error(errno);
    return ret;
  }

  template <typename T, typename Buffer>
  inline ssize_t writev(Handle<T>& h, Buffer const& buf, size_t count)
  {
    // TODO: think about raw_buffer_vec
    ssize_t ret = ::writev(h.raw_handle(), raw_buffer_vec(buf), count);
    if (ret == -1) h.set_error(errno);
    return ret;
  }

  template <typename T, typename Buffer>
  inline ssize_t readv(Handle<T>& h, Buffer& buf, size_t count)
  {
    // TODO: think about raw_buffer_vec
    ssize_t ret = ::readv(h.raw_handle(), raw_buffer_vec(buf), count);
    if (ret == -1) h.set_error(errno);
    return ret;
  }

};

} } } // namespace ku::net::ops

