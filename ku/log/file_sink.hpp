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
  FileSink(char const* path, char const* base_name, LogLevel log_level = LogLevel::Debug);
  virtual ~FileSink() { close(); }

  virtual void write(BufferList const& list);

  void set_size_limit(size_t limit) { size_limit_ = limit; }

private:
  std::string file_name(int seq_no = 0) const;
  void open();
  void close();
  void rotate();

private:
  int file_handle_;
  int seq_no_;
  std::string path_, base_name_;
  size_t size_, size_limit_;
};

} } // namespace ku::log

