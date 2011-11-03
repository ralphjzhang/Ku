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
#include "buffer.hpp"
#include "file_sink.hpp"

namespace ku { namespace log {

FileSink::FileSink(char const* path)
{
  file_handle_ = ::open(path, O_CREAT | O_CLOEXEC);
}

void FileSink::write(Buffer& buf)
{
  ::writev(file_handle_, buf.raw_buffer(), buf.raw_buffer_count());
}

void FileSink::close()
{
  if (file_handle_)
    ::close(file_handle_);
}

} } // namespace ku::log

