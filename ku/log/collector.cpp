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
  message_.append(now.c_str(), now.size());
  message_.set_log_level(log_level);
  char const* s_log_level = to_log(log_level);
  message_.append(s_log_level, std::strlen(s_log_level)); 
  return *this;
}

Collector::~Collector()
{
  message_.append('\n');
  logger_.submit(std::move(message_));
}

} } // namespace ku::log

