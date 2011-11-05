/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstring>
#include <string>
#include <type_traits>
#include "util.hpp"
#include "log_level.hpp"
#include "buffer.hpp"
#include "logger.hpp"

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

  Collector(Collector&& col) { }//{ buffer_.swap(col.buffer_); }

  void append(char c) { buffer().append(c); }
  void append(std::string const& s) { buffer().append(s.c_str(), s.size()); }
  void append(char const* s, size_t size) { buffer().append(s, size); }

  void set_format(char const* fmt) { format_ = fmt; }

  // C printf style collecting support
  template <typename... Args>
  Collector& operator () (char const* fmt, Args... args);

  Collector& self() { return *this; }

private:
  Buffer& buffer() { return buffer_; }
  void submit();

private:
  Buffer buffer_;
  char const* format_;
};

// =======================================================================================
// C++ stream style collecting support.
// User can support custome type collecting by providing to_str() function for the type,
// or if doing so being more efficient, provide custom opeartor <<
// =======================================================================================
template <typename T>
Collector& operator << (Collector& c, T const& t)
{
  c.append(to_log(t));
  return c;
}

inline Collector& operator << (Collector& c, char ch)
{
  c.append(ch);
  return c;
}

inline Collector& operator << (Collector& c, char const* s)
{
  c.append(s, std::strlen(s));
  return c;
}

inline Collector& operator << (Collector& c, std::string const& s)
{
  c.append(s);
  return c;
}

template <typename T>
auto operator << (Collector& c, T t)
  -> typename std::enable_if<std::is_unsigned<T>::value, Collector&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%llu", util::implicit_cast<unsigned long long>(t));
  c.append(buf, len);
  return c;
}

template <typename T>
auto operator << (Collector& c, T t)
  -> typename std::enable_if<std::is_signed<T>::value, Collector&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%lld", util::implicit_cast<unsigned long long>(t));
  c.append(buf, len);
  return c;
}

template <typename T>
auto operator << (Collector& c, T t)
  -> typename std::enable_if<std::is_floating_point<T>::value, Collector&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%f", util::implicit_cast<unsigned long long>(t));
  c.append(buf, len);
  return c;
}

// =======================================================================================
// C printf style collecting support
// Usage like:
//   collector("format string: answer to life, %s and everything: %d", "universe", 42); 
// User support custom type collecting by providing to_log() function for the type
// =======================================================================================
template <typename... Args>
Collector& Collector::operator () (char const* fmt, Args... args)
{
  // For logging, it's fine to just store the pointer, collector won't live over the line.
  set_format(fmt);
  // TODO
  return *this;
}

inline Collector collector(LogLevel log_level)
{
  return Collector(log_level);
}

} } // namespace ku::log

