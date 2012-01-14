/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <vector>
#include "util.hpp"
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

template <typename Entry, typename Claimer, typename Waiting>
class RingBuffer
{
public:
  RingBuffer(size_t size)
    : cursor_(0), mask_(next_pow_of_two(size) - 1), entries_(mask_ + 1)
    , claimer_(capacity())
  {}

  size_t capacity() const { return entries_.size(); }
  size_t cursor() const { return cursor_; }

  Entry& get(size_t seq) { return entries_[seq & mask_]; }
  Entry const& get(size_t seq) const { return entries_[seq & mask_]; }
  Entry& operator[] (size_t seq) { return get(seq); }
  Entry const& operator[] (size_t seq) const { return get(seq); }

  size_t next() { return claimer_.get_next(gating_seqs_); }
  size_t claim(size_t seq) { return claimer_.claim(seq, gating_seqs_); }
  bool has_available(size_t capacity) { return claimer_.has_available(capacity, gating_seqs_); }

private:
  void wait_for(size_t capacity) { claimer_.wait_for_capacity(capacity, gating_seqs_); }
  void publish(size_t seq) { claimer_.publish(seq, cursor_); waiting_.notify_all(); }

private:
  volatile size_t cursor_;
  size_t mask_;
  SequenceList gating_seqs_; // TODO initializing this should be enforced(or checked)
  std::vector<Entry> entries_;
  Claimer claimer_;
  Waiting waiting_;
};


} } } // namespace ku::fusion::disruptor

