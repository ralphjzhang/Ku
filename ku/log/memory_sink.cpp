/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <sys/uio.h>
#include "buffer_list.hpp"
#include "memory_sink.hpp"

namespace ku { namespace log {

// It is by intention this function does not free memory, this class is for testing ONLY.
// In production it'll eat up memory with every log, and becomes slow, accidental usage is
// thus easier to get noticed.
void MemorySink::write(BufferList const& list)
{
  iovec const* p = list.raw_buffer();
  for (uint32_t n = 0; n < list.raw_buffer_count(); ++n)
    data_.emplace_back(static_cast<char const*>(p[n].iov_base), p[n].iov_len);
}

} } // namespace ku::log

