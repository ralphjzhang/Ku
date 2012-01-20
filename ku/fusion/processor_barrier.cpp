/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <limits>
#include <algorithm>
#include "processor_barrier.hpp"
#include "sequence.hpp"

namespace ku { namespace fusion {

size_t ProcessorBarrier::max_sequence() const
{
  assert(!processor_seqs_.empty());
  size_t max = 0;
  for (auto const seq_ptr : processor_seqs_) {
    size_t const value = seq_ptr->get();
    max = std::max(max, value);
  }
  return max;
}

size_t ProcessorBarrier::min_sequence() const
{
  assert(!processor_seqs_.empty());
  size_t min = std::numeric_limits<size_t>::max();
  for (auto const seq_ptr : processor_seqs_) {
    size_t const value = seq_ptr->get();
    min = std::min(min, value);
  }
  return min;
}


} } // namespace ku::fusion
