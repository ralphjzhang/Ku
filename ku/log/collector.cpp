/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "collector.hpp"
#include "logger.hpp"

namespace ku { namespace log {

Collector::Collector(LogLevel log_level)
{
  buffer_.set_log_level(log_level);
  unsigned long line_no = util::next_seq<util::LineNo>();
}

void Collector::submit()
{
  logger().submit(buffer_);
}

} } // namespace ku::log

