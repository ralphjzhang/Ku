/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <forward_list>
#include "sink.hpp"
#include "util.hpp"

namespace ku { namespace log {

class Buffer;

class Writer : private util::noncopyable
{
  typedef std::forward_list<Sink_ptr> SinkList;

public:
  Writer() = default;

  void add_sink(Sink_ptr sink) { sink_list_.push_front(std::move(sink)); }
  void write(Buffer& buf);

private:
  SinkList sink_list_;
};

} } // namespace ku::log


