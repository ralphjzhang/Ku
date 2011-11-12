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

namespace {
inline size_t to_str(char* buf, unsigned num, size_t size)
{
  size_t sz = size;
  while (sz) {
    buf[--sz] = num % 10 + '0';
    num /= 10;
  }
  return size;
}
} // unamed namespace

namespace ku { namespace log { namespace util {

size_t now(char* buf)
{
  timespec ts;
  ::clock_gettime(CLOCK_REALTIME, &ts);
  tm t;
  ::localtime_r(&ts.tv_sec, &t);
  char* p = buf;
  p += to_str(p, t.tm_year + 1900, 4);
  *p++ = '-';
  p += to_str(p, t.tm_mon + 1, 2);
  *p++ = '-';
  p += to_str(p, t.tm_mday, 2);
  *p++ = ' ';
  p += to_str(p, t.tm_hour, 2);
  *p++ = ':';
  p += to_str(p, t.tm_min, 2);
  *p++ = ':';
  p += to_str(p, t.tm_sec, 2);
  *p++ = '.';
  p += to_str(p, ts.tv_nsec, 9);
  return p - buf;
}

std::string now()
{
  char buf[32];
  now(buf);
  return std::string(buf);
}

} } } // namespace ku::log::util

