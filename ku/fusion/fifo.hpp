/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "handle.hpp"
#include "ops/common.hpp"

namespace ku { namespace fusion {

namespace ops {
struct Fifo;
} // namespace ku::fusion::ops

class Fifo
{
  using HandleType = Handle<ops::Fifo>;

public:
  Fifo(char const* path);
  Fifo(Fifo&& f) : handle_(std::move(f.handle_)) { }
  ~Fifo() = default;

  ssize_t read(uint64_t& val, size_t)
  { return ops::Common::read(handle_, &val, sizeof(val)); }

  ssize_t write(uint64_t val, size_t)
  { return ops::Common::write(handle_, &val, sizeof(val)); }

private:
  HandleType handle_;
};

} } // namespace ku::fusion

