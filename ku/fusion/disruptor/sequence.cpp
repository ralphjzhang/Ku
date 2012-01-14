/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <limits>
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

size_t SequenceList::min_sequence() const
{
  size_t min = std::numeric_limits<size_t>::max();
  for (auto const& seq : list_) {
    size_t value = seq.get();
    min = min < value ? min : value;
  }
  return min;
}


} } } // namespace ku::fusion::disruptor

