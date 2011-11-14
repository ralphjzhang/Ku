/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "ops/fifo.hpp"
#include "fifo.hpp"

namespace ku { namespace fusion {

Fifo::Fifo(char const* path) : handle_(ops::Fifo::create(path))
{
}

} } // namespace ku::fusion

