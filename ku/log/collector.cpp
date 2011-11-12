/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "collector.hpp"
#include "logger.hpp"

namespace ku { namespace log {

Collector::Collector(LogLevel log_level, BufferList& free_queue, Logger& logger)
  : logger_(logger), message_(log_level, free_queue)
{
  char buf[32];
  size_t sz = util::now(buf);
  message_.append(buf, sz);
  char const* s_log_level = to_log(message_.log_level());
  message_.append(s_log_level, std::strlen(s_log_level)); 
}

Collector::~Collector()
{
  message_.append('\n');
  logger_.submit(std::move(message_));
}

} } // namespace ku::log

