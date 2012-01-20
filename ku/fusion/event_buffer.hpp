/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/epoll.h>
#include <vector>
#include <functional>
#include "sequence.hpp"
#include "processor_barrier.hpp"

namespace ku { namespace fusion {

/// Disruptor ring buffer, specifically made for epoll events
//  Only one writer is allowed (which is the epoll poller)
class EventBuffer
{
  typedef std::vector<epoll_event> Events;

public:
  EventBuffer(size_t size);
  ~EventBuffer() = default;

  ProcessorBarrier& barrier() { return barrier_; }

  template <typename F>
  void set_wait_strategy(F const& wait_function)
  {
    wait_strategy_ = wait_function;
  }

  size_t const initial_sequence() const { return mask_; }
  size_t capacity() const { return events_.size(); }

  size_t claimed_sequence() const { return claimed_seq_.get(); }
  size_t published_sequence() const { return published_seq_.get(); }

  epoll_event& raw_event(size_t seq) { return events_[seq & mask_]; }
  epoll_event const& raw_event(size_t seq) const { return events_[seq & mask_]; }

  void claim(size_t seq);
  void publish(size_t seq);

  // the max sequence that can be claimed without clashing with processors
  size_t max_usable_sequence() const;
  // the max usable sequence in the forward continous block
  size_t max_usable_block_sequence() const;

private:
  void wait_for_seq(size_t seq);

private:
  size_t const mask_;
  Events events_;
  ProcessorBarrier barrier_;
  Sequence claimed_seq_, published_seq_, gating_seq_;
  std::function<void(size_t)> wait_strategy_;
};

} } // namespace ku::fusion

