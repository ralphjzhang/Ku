/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <atomic>
#include <chrono>
#include <string>

namespace ku { namespace log { namespace util {

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}

struct noncopyable
{
  noncopyable() = default;
  ~noncopyable() = default;
  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
};

inline std::string now(bool date = true)
{
  auto tm = std::chrono::system_clock::now();
}

enum SeqType { LineNo };

template <SeqType S>
unsigned long next_seq()
{
  static std::atomic<unsigned long> seq(0);
  return ++seq;
}

} } } // namespace ku::log::util

