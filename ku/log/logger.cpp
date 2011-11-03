/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "buffer.hpp"
#include "logger.hpp"

namespace ku { namespace log {

void Logger::submit(Buffer& buf)
{
  writer_.write(buf);
}

Logger& logger()
{
  // TODO this should be thread_local
  static /*thread_local*/ Logger lg;
  return lg;
}

} } // namespace ku::log

