/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <thread>
#include <algorithm>
#include "event_buffer.hpp"
#include "processor_barrier.hpp"
#include "util.hpp"

namespace ku { namespace fusion {

EventBuffer::EventBuffer(size_t size) 
  : mask_(util::ceiling_pow_of_two(size) - 1), events_(mask_ + 1)
  , claimed_seq_(capacity() - 1)
  , published_seq_(capacity() - 1) 
  , gating_seq_(capacity() - 1)
  , wait_strategy_([](size_t){ std::this_thread::yield(); })
{
  // sequence counters start from 1 slot before capacity to make wrapping easy
}

void EventBuffer::claim(size_t seq)
{
  assert(seq >= claimed_sequence());
  claimed_seq_.set(seq);
  wait_for_seq(seq);
}

void EventBuffer::publish(size_t seq)
{
  published_seq_.set(seq);
  // TODO barrier_.notify_all();
}

size_t EventBuffer::max_usable_sequence() const
{
  // In a ring buffer setup, the max usable sequence is the sequence of the slowest processor
  return barrier_.min_sequence() + capacity();
}

size_t EventBuffer::max_usable_block_sequence() const
{
  // block_end_sequence: the sequence that reaches the end of buffer without wrapping
  size_t block_end_sequence = (published_sequence() + 1 + capacity() & ~mask_) - 1;
  return std::min(max_usable_sequence(), block_end_sequence);
}

void EventBuffer::wait_for_seq(size_t seq)
{
  size_t const wrap_point = seq - capacity();
  if (wrap_point > gating_seq_.get()) {
    size_t min_seq;
    size_t wait_count = 0;
    while (wrap_point > (min_seq = barrier_.min_sequence()))
      wait_strategy_(wait_count++);
    gating_seq_.set(min_seq);
  }
}


} } // namespace ku::fusion

