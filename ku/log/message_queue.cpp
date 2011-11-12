/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "sink.hpp"
#include "message_queue.hpp"

namespace ku { namespace log {

void MessageQueue::flush_to(Sink& sink)
{
  // If the whole buffers_'s log_level is no less than this sink, write the buffer directly,
  // otherwise, copy out those nodes that log_level is no less than the sink, then write the copy.
  if (min_log_level_ >= sink.log_level()) {
    sink.write(buffers_);
  } else {
    BufferList bufs;
    bufs.reserve(buffers_.raw_buffer_count());
    Buffer::Node const* node_ptr = reinterpret_cast<Buffer::Node const*>(buffers_.raw_buffer());
    for (MessageInfo const& info : index_) {
      if (info.log_level >= sink.log_level())
        bufs.push_back(node_ptr, info.raw_buffer_count);
      node_ptr += info.raw_buffer_count;
    }
    // This one-shot write assumes sink uses blocking write,
    // or non-blocking but handle partial write internally
    sink.write(bufs);
  }
}

} } // namespace ku::log

