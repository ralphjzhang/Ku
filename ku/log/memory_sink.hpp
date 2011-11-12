/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <vector>
#include <string>
#include "sink.hpp"

namespace ku { namespace log {

// Writing to memory, mainly for testing purpose
class MemorySink : public Sink
{
  typedef std::vector<std::string> StringList;

public:
  MemorySink(LogLevel log_level) : Sink(log_level) { }

  virtual void write(BufferList const& list);

private:
  StringList data_;
};

} } // namespace ku::log

