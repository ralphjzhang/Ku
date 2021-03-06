/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

class Claimer
{
public:
  Claimer(size_t buf_size);
  ~Claimer() = default;

  size_t get() const { return claim_seq_.get(); }
  bool has_available(size_t capacity, SequenceList const& seq_list);
  size_t claim_next(SequenceList const& seq_list);
  size_t claim_next(size_t incr, SequenceList const& seq_list);

  void wait_for_capacity(size_t capacity, SequenceList const& seq_list);

private:
  void wait_for_seq(size_t seq, SequenceList const& seq_list);

private:
  size_t const buf_size_;
  Sequence claim_seq_, gating_seq_;
};


} } } // namespace ku::fusion::disruptor

