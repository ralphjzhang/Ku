/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../util.hpp"
#include "../handle.hpp"

namespace ku { namespace fusion { namespace ops {

struct Fifo
{
  static inline Handle<Fifo> create(char const* path, bool nonblock = true)
  {
    if (::mkfifo(path, 0666) == -1)
      throw std::system_error(util::errc(), "ops::Fifo::create");
    int fd = ::open(path, nonblock ? O_RDWR | O_NONBLOCK : O_RDWR);
    if (fd == -1)
      throw std::system_error(util::errc(), "ops::Fifo::create");
    return Handle<Fifo>(fd);
  }
};

} } } // namespace ku::fusion::ops

