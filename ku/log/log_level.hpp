/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstdint>

namespace ku { namespace log {

enum class LogLevel : uint32_t 
{
  Debug, Info, Warn, Error, Fatal
};

LogLevel& g_log_level();

char const* to_log(LogLevel level);

} } // namespace ku::log

