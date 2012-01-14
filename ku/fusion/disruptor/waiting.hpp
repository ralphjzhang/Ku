/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstddef>

namespace ku { namespace fusion { namespace disruptor {

class BlockingWaiting
{
public:
  void notify_all();
};

} } } // namespace ku::fusion::disruptor

