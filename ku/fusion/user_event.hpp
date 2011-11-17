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
  typedef Handle<ops::UserEvent> HandleType;

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

// TODO handle subscriber join/leave in the middle
class PublisherUserEvent
{
  typedef Handle<ops::UserEvent> HandleType;
  friend class SubscriberUserEvent;

public:
  PublisherUserEvent();
  HandleType const& handle() const { return handle_; }

  void initialize()
  {
    uint64_t val = UINT64_MAX - 1;
    ops::Common::write(handle_, &val, sizeof(val));
  }

  bool try_publish();

private:
  inline void increment() { ++subscribers_; }
  inline void decrement() { --subscribers_; }

private:
  bool initialized_;
  HandleType handle_;
  std::atomic_uint_fast64_t subscribers_;
};

class SubscriberUserEvent
{
  typedef Handle<ops::UserEvent> HandleType;

public:
  SubscriberUserEvent(PublisherUserEvent& p) : publisher_(p) { publisher_.increment(); }
  ~SubscriberUserEvent() { publisher_.decrement(); }

  HandleType const& handle() const { return publisher_.handle(); }

  bool try_read();

private:
  PublisherUserEvent& publisher_;
};

} } // namespace ku::fusion

