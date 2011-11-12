/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include "buffer_list.hpp"
#include "console_sink.hpp"

namespace ku { namespace log {

void ConsoleSink::write(BufferList const& list)
{
  ::writev(STDOUT_FILENO, list.raw_buffer(), list.raw_buffer_count());
}

} } // namespace ku::log

