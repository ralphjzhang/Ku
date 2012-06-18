/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstdint>
#include <atomic>
#include <cassert>
#include "handle.hpp"
#include "ops/common.hpp"

namespace ku { namespace fusion {

namespace ops {
struct UserEvent;
} // namespace ku::fusion::ops


class UserEvent
{
  using HandleType = Handle<ops::UserEvent>;

public:
  UserEvent(unsigned init_value, bool non_block = true, bool semaphore = false);
  ~UserEvent() = default;

  HandleType const& handle() const { return handle_; }

  ssize_t read(uint64_t& val, size_t = 0)
  { return ops::Common::read(handle_, &val, sizeof(val)); }

  ssize_t write(uint64_t val, size_t = 0)
  { return ops::Common::write(handle_, &val, sizeof(val)); }

  void close() { handle_.close(); }

private:
  HandleType handle_;
};

class WriterUserEvent
{
  using HandleType = Handle<ops::UserEvent>;
  friend class ReaderUserEvent;

public:
  WriterUserEvent();
  HandleType const& handle() const { return handle_; }

  void write();

private:
  bool initialized_;
  HandleType handle_;
};

class ReaderUserEvent
{
  using HandleType = Handle<ops::UserEvent>;

public:
  ReaderUserEvent(WriterUserEvent& writer) : handle_(writer.handle_) { }
  ~ReaderUserEvent() = default;

  HandleType const& handle() const { return handle_; }
  void read();

private:
  HandleType& handle_;
};

} } // namespace ku::fusion

