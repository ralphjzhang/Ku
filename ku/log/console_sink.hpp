/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "sink.hpp"

namespace ku { namespace log {

class ConsoleSink : public Sink
{
public:
  ConsoleSink(LogLevel log_level = LogLevel::Debug) : Sink(log_level) { }
  virtual void write(BufferList const& list);

  virtual ~ConsoleSink() { }
};

} } // namespace ku::log

