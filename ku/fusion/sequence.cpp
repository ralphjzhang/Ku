/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include <limits>
#include "sequence.hpp"

namespace ku { namespace fusion {

size_t SequenceList::min_sequence() const
{
  assert(!list_.empty());
  size_t min = std::numeric_limits<size_t>::max();
  for (auto const seq_ptr : list_) {
    size_t const value = seq_ptr->get();
    min = min < value ? min : value;
  }
  return min;
}

void SequenceList::initialize(std::initializer_list<Sequence*> const& seqs)
{
  assert(list_.empty());
  assert(seqs.size() > 0);
  for (auto seq_ptr : seqs)
    list_.push_back(seq_ptr);
}

} } // namespace ku::fusion

