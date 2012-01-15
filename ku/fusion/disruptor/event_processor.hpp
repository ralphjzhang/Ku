/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "sequence.hpp"

namespace ku { namespace fusion { namespace disruptor {

class EventProcessor
{
public:
  virtual Sequence& sequence() = 0;
};

} } } // namespace ku::fusion::disruptor

