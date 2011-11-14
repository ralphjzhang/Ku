/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "signal.hpp"
#include "ops/signal.hpp"

namespace ku { namespace fusion {

Signal::Signal()
  : handle_(ops::Signal::create({ SIGINT })) //TODO parameterize signal set
{
}


} } // namespace ku::fusion

