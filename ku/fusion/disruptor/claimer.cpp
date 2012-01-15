/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <thread>
#include "claimer.hpp"

namespace ku { namespace fusion { namespace disruptor {

Claimer::Claimer(size_t buf_size)
  : buf_size_(buf_size), claim_seq_(buf_size - 1), gating_seq_(buf_size - 1)
{
  // It's a simple trick initializing claim_seq_/gating_seq_ as (buf_size - 1), 
  // that makes wrapping calculation easy, without using signed sequence
}

size_t Claimer::claim_next(SequenceList const& seq_list)
{
  size_t const next_seq = claim_seq_.get() + 1ULL;
  claim_seq_.set(next_seq);
  wait_for_seq(next_seq, seq_list);
  return next_seq;
}

size_t Claimer::claim_next(size_t incr, SequenceList const& seq_list)
{
  size_t const next_seq = claim_seq_.get() + incr;
  claim_seq_.set(next_seq);
  wait_for_seq(next_seq, seq_list);
  return next_seq;
}

bool Claimer::has_available(size_t capacity, SequenceList const& seq_list)
{
  assert(capacity > 0 && capacity <= buf_size_);

  size_t const wrap_point = claim_seq_.get() + capacity - buf_size_;
  if (wrap_point > gating_seq_.get()) {
    size_t const min_seq = seq_list.min_sequence();
    gating_seq_.set(min_seq);
    return wrap_point <= min_seq;
  }
  return true;
}

void Claimer::wait_for_capacity(size_t capacity, SequenceList const& seq_list)
{
  while (!has_available(capacity, seq_list))
    std::this_thread::yield();
}

void Claimer::wait_for_seq(size_t seq, SequenceList const& seq_list)
{
  size_t const wrap_point = seq - buf_size_;
  if (wrap_point > gating_seq_.get()) {
    size_t min_seq;
    while (wrap_point > (min_seq = seq_list.min_sequence()))
      std::this_thread::yield();
    gating_seq_.set(min_seq);
  }
}

} } } // namespace ku::fusion::disruptor

