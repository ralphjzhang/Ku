/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "writer.hpp"

namespace ku { namespace log {

class Buffer;

class Logger
{
public:
  Logger() = default;
  Writer& writer() { return writer_; }

  void submit(Buffer& buf);

private:
  Writer writer_;
};

Logger& logger();

} } // namespace ku::log

