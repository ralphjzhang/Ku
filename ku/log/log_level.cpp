/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "log_level.hpp"

namespace ku { namespace log {

char const* to_log(LogLevel level)
{
  switch (level) {
  case LogLevel::Debug:
    return " Debug ";
  case LogLevel::Info:
    return " Info  ";
  case LogLevel::Warn:
    return " Warn  ";
  case LogLevel::Error:
    return " Error ";
  case LogLevel::Fatal:
    return " Fatal ";
  default:
    return "Unknown";
  }
}

} } // namespace ku::log

