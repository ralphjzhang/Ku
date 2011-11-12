/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstring>
#include <string>
#include <type_traits>
#include "util.hpp"
#include "log_level.hpp"
#include "message.hpp"
#include "buffer_list.hpp"

namespace ku { namespace log {

class Logger;

// =======================================================================================
// Collector is the logger front end.
// It collects log data to a buffer, and submit for further processing.
// =======================================================================================
class Collector : private util::noncopyable
{
public:
  Collector() = delete;
  Collector(LogLevel log_level, BufferList& free_queue, Logger& logger);
  Collector(Collector&& col) : logger_(col.logger_), message_(std::move(col.message_)) { }

  ~Collector();

  Message& message() { return message_; }

private:
  Logger& logger_;
  Message message_;
};

} } // namespace ku::log

