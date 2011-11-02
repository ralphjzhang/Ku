/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <array>
#include <cstring>

namespace ku { namespace log {

// =======================================================================================
// Collector is the logger front end.
// It collects log data to a buffer, and submit for further processing.
// =======================================================================================
class Collector
{
public:
  Collector() : end_(buffer_.begin()), count_(1) { buffer_[0] = '\n'; }
  ~Collector() { submit(); }

  Collector(Collector const&) = delete;
  Collector(Collector&& col) : buffer_(std::move(col.buffer_)), end_(col.end_), count_(col.count_)
  { }

  void append(std::string const& s)
  {
    count_ += s.size();
    if (count_ >= 1024)
      ; // TODO resize
    end_ = strcpy(end_, s.c_str());
    *end_ = '\n';
  }

  template <typename Int>
  void append(Int i)
  {
    // TODO
  }

private:
  void submit();

private:
  std::array<char, 1024> buffer_;
  char *end_;
  size_t count_;
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

inline Collector get_collector()
{
  return Collector();
}

} } // namespace ku::log

