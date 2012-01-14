/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

class SinglePublisherClaimer
{
public:
  SinglePublisherClaimer(size_t buf_size)
    : buf_size_(buf_size), claim_seq_(0), gating_seq_(0)
  {}
  ~SinglePublisherClaimer() = default;

  size_t get() const { return claim_seq_.get(); }
  size_t get_next(SequenceList const& seq_list);
  size_t get_next(size_t incr, SequenceList const& seq_list);
  size_t claim(size_t seq, SequenceList const& seq_list);
  bool has_available(size_t capacity, SequenceList const& seq_list);

  void publish(size_t seq, Sequence& cursor);

  void wait_for_capacity(size_t capacity, SequenceList const& seq_list);

private:
  void wait_for_seq(size_t seq, SequenceList const& seq_list);

private:
  size_t buf_size_;
  Sequence claim_seq_, gating_seq_;
};


} } } // namespace ku::fusion::disruptor

