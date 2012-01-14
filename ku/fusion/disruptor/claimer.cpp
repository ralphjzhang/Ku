/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <thread>
#include "claimer.hpp"

namespace ku { namespace fusion { namespace disruptor {

size_t SinglePublisherClaimer::get_next(SequenceList const& seq_list)
{
  const size_t next_seq = claim_seq_.get() + 1UL;
  claim_seq_.set(next_seq);
  wait_for_seq(next_seq, seq_list);
  return next_seq;
}

size_t SinglePublisherClaimer::get_next(size_t incr, SequenceList const& seq_list)
{
  const size_t next_seq = claim_seq_.get() + incr;
  claim_seq_.set(next_seq);
  wait_for_seq(next_seq, seq_list);
  return next_seq;
}

size_t SinglePublisherClaimer::claim(size_t seq, SequenceList const& seq_list)
{
  claim_seq_.set(seq);
  wait_for_seq(seq, seq_list);
  return seq;
}

bool SinglePublisherClaimer::has_available(size_t capacity, SequenceList const& seq_list)
{
  const size_t wrap_point = claim_seq_.get() + capacity - buf_size_;
  if (wrap_point > gating_seq_.get()) {
    const size_t min_seq = seq_list.min_sequence();
    gating_seq_.set(min_seq);
    return wrap_point <= min_seq;
  }
  return true;
}

void SinglePublisherClaimer::publish(size_t seq, Sequence& cursor)
{
  cursor.set(seq);
}

void SinglePublisherClaimer::wait_for_capacity(size_t capacity, SequenceList const& seq_list)
{
  while (!has_available(capacity, seq_list))
    std::this_thread::yield();
}

void SinglePublisherClaimer::wait_for_seq(size_t seq, SequenceList const& seq_list)
{
  const size_t wrap_point = seq - buf_size_;
  if (wrap_point > gating_seq_.get()) {
    size_t min_seq;
    while (wrap_point > (min_seq = seq_list.min_sequence()))
      std::this_thread::yield();
    gating_seq_.set(min_seq);
  }
}

} } } // namespace ku::fusion::disruptor

