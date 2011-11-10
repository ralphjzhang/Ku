/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <cassert>
#include "buffer_queue.hpp"
#include "file_sink.hpp"

#include <iostream>

namespace ku { namespace log {

FileSink::FileSink(char const* path)
{
  file_handle_ = ::open(path, O_CREAT | O_CLOEXEC, 0777);
  assert(file_handle_ > 0);
  if (file_handle_ < 0)
    file_handle_ = 0;
}

void FileSink::write(BufferQueue const& queue)
{
  ::writev(file_handle_, queue.raw_buffer(), queue.raw_buffer_count());
}

void FileSink::close()
{
  if (file_handle_) {
    ::fsync(file_handle_);
    ::close(file_handle_);
  }
}

} } // namespace ku::log

