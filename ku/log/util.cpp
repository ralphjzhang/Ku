/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include "util.hpp"

namespace ku { namespace log { namespace util {

std::string now()
{
  timeval tv;
  ::gettimeofday(&tv, NULL);
  char buf[32];
  if (tm* tm_ptr = localtime(&tv.tv_sec)) {
    size_t s = std::strftime(buf, 32, "%Y-%m-%d %H:%M:%S.", tm_ptr);
    std::sprintf(buf + s, "%06ld", tv.tv_usec);
  }
  return std::string(buf);
}

} } } // namespace ku::log::util

