/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <initializer_list>
#include <vector>
#include "util.hpp"
#include "sequence.hpp"
#include "claimer.hpp"

namespace ku { namespace fusion { namespace disruptor {

template <typename Event, typename Waiting>
class RingBuffer
{
public:
  using EventType Event;

  RingBuffer(size_t size)
    : mask_(next_pow_of_two(size) - 1), entries_(mask_ + 1)
    , cursor_(capacity() - 1) // cursor starts from 1 slot before capacity to make wrapping easy, consider moving to Claimer
    , claimer_(capacity())
  {}

  void set_gatings(std::initializer_list<Sequence*> const& seqs) { gating_seqs_.initialize(seqs); }

  size_t capacity() const { return entries_.size(); }
  size_t cursor() const { return cursor_.get(); }

  Event& get(size_t seq) { return entries_[seq & mask_]; }
  Event const& get(size_t seq) const { return entries_[seq & mask_]; }
  Event& operator[] (size_t seq) { return get(seq); }
  Event const& operator[] (size_t seq) const { return get(seq); }

  size_t claim_next() { return claimer_.claim_next(gating_seqs_); }
  size_t claim_next(size_t incr) { return claimer_.claim_next(incr, gating_seqs_); }
  bool has_available(size_t capacity) { return claimer_.has_available(capacity, gating_seqs_); }
  void publish(size_t seq) { cursor_.set(seq); waiting_.notify_all(); }

private:
  void wait_for(size_t capacity) { claimer_.wait_for_capacity(capacity, gating_seqs_); }

private:
  size_t const mask_;
  std::vector<Event> entries_;
  Sequence cursor_;
  SequenceList gating_seqs_;
  Claimer claimer_;
  Waiting waiting_;
};


} } } // namespace ku::fusion::disruptor

