/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstddef>
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

class ConditionWaiting
{
public:
  void notify_all();
};

class YieldWaiting
{
public:
  void wait_for(size_t seq, Sequence const& cursor, SequenceList const& seq_list);
  void notify_all() {}
};

} } } // namespace ku::fusion::disruptor

