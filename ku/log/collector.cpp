/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "collector.hpp"
#include "logger.hpp"

namespace ku { namespace log {

Collector& Collector::initialize(LogLevel log_level)
{
  std::string now(util::now());
  buffer().append(now.c_str(), now.size());
  buffer().set_log_level(log_level);
  char const* s_log_level = to_log(log_level);
  buffer().append(s_log_level, std::strlen(s_log_level)); 
  return *this;
}

void Collector::submit()
{
  buffer().append('\n');
  logger().submit(buffer());
}

} } // namespace ku::log

