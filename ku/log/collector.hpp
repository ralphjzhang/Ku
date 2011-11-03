/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <string>
#include "util.hpp"
#include "log_level.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

// =======================================================================================
// Collector is the logger front end.
// It collects log data to a buffer, and submit for further processing.
// =======================================================================================
class Collector : private util::noncopyable
{
public:
  Collector() = default;
  Collector(LogLevel log_level);

  ~Collector() { submit(); }

  Collector(Collector&& col) { buffer_.swap(col.buffer_); }

  void append(std::string const& s) { buffer_.append(s.c_str(), s.size()); }

  template <typename Int>
  void append(Int i)
  {
    // TODO
  }

private:
  void submit();

private:
  Buffer buffer_;
};

template <typename T>
Collector& operator << (Collector& c, T const& t)
{
  c.append(t);
  return c;
}

template <typename T>
Collector& operator << (Collector& c, T&& t)
{
  // TODO optimize for rvalue?
  return c;
}

template <typename T>
Collector& operator , (Collector& c, T const& t)
{
  // TODO: printf style
  return c;
}

inline Collector collector()
{
  return Collector();
}

} } // namespace ku::log

