/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "sink.hpp"

namespace ku { namespace log {

class FileSink : public Sink
{
public:
  FileSink(char const* path);

  virtual ~FileSink() { close(); }

  virtual void write(Buffer& buf);

private:
  void close();

  int file_handle_;
};

} } // namespace ku::log

