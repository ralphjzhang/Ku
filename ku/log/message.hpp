/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "log_level.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

class Message : private util::noncopyable
{
public:
  Message() = delete;
  Message(LogLevel log_level, BufferList& free_queue)
    : log_level_(log_level), buffer_(free_queue) { }
  Message(Message&& message)
    : log_level_(message.log_level_), buffer_(std::move(message.buffer_)) { }

  iovec const* raw_buffer() const { return buffer_.raw_buffer(); }
  size_t raw_buffer_count() const { return buffer_.raw_buffer_count(); }

  Buffer& buffer() { return buffer_; }

  void append(char const* str, size_t count) { buffer_.append(str, count); }
  void append(char c) { buffer_.append(c); }

  // C printf style collecting support
  template <typename... Args>
  Message& operator () (char const* fmt, Args... args);

  LogLevel log_level() { return log_level_; }

private:
  LogLevel log_level_;
  Buffer buffer_;
};

// =======================================================================================
// C++ stream style collecting support.
// User can support custome type collecting by providing to_str() function for the type,
// or if doing so being more efficient, provide custom opeartor <<
// =======================================================================================
/*
template <typename T>
Message& operator << (Message& m, T const& t)
{
  m.append(to_log(t));
  return m;
}

inline Message& operator << (Message& m, char ch)
{
  m.append(ch);
  return m;
}*/

inline Message& operator << (Message& m, char const* s)
{
  m.append(s, std::strlen(s));
  return m;
}

inline Message& operator << (Message& m, std::string const& s)
{
  m.append(s.c_str(), s.size());
  return m;
}

template <typename T>
auto operator << (Message& m, T t)
  -> typename std::enable_if<std::is_unsigned<T>::value, Message&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%llu", util::implicit_cast<unsigned long long>(t));
  m.append(buf, len);
  return m;
}

template <typename T>
auto operator << (Message& m, T t)
  -> typename std::enable_if<std::is_signed<T>::value, Message&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%lld", util::implicit_cast<unsigned long long>(t));
  m.append(buf, len);
  return m;
}

template <typename T>
auto operator << (Message& m, T t)
  -> typename std::enable_if<std::is_floating_point<T>::value, Message&>::type
{
  char buf[32];
  size_t len = sprintf(buf, "%f", util::implicit_cast<unsigned long long>(t));
  m.append(buf, len);
  return m;
}

// =======================================================================================
// C printf style collecting support
// Usage like:
//   message("format string: answer to life, %s and everything: %d", "universe", 42); 
// User support custom type collecting by providing to_log() function for the type
// =======================================================================================
template <typename... Args>
Message& Message::operator () (char const* fmt, Args... args)
{
  // TODO
  return *this;
}

} } // namespace ku::log

