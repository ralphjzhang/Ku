/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "buffer.hpp"
#include "writer.hpp"

namespace ku { namespace log {

void Writer::write(Buffer& buf)
{
  for (auto& sink_ptr : sink_list_) {
    if (buf.log_level() >= sink_ptr->log_level())
      sink_ptr->write(buf);
  }
}

} } // namespace ku::log

