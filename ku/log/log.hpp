/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "logger.hpp"
#include "collector.hpp"

#define LOG(level) \
  if (LogLevel::level < g_logger().log_level()); \
  else g_logger().create_collector(LogLevel::level).message()

#define LOG_IF(level, cond) \
  if (LogLevel::level < g_logger().log_level() || !(cond)); \
  else g_logger().create_collector(LogLevel::level).message()


