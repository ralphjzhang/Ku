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

size_t now(char* buf)
{
  timespec ts;
  ::clock_gettime(CLOCK_REALTIME, &ts);
  tm t;
  ::localtime_r(&ts.tv_sec, &t);
  return sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d.%09lu", t.tm_year + 1900, t.tm_mon + 1,
      t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, ts.tv_nsec);
}

std::string now()
{
  char buf[32];
  now(buf);
  return std::string(buf);
}

} } } // namespace ku::log::util

