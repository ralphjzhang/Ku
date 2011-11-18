/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <system_error>
#include "ops/user_event.hpp"
#include "util.hpp"
#include "user_event.hpp"

namespace ku { namespace fusion {

/// UserEvent ///
//
UserEvent::UserEvent(unsigned init_value, bool non_block, bool semaphore)
  : handle_(ops::UserEvent::create(init_value, non_block, semaphore))
{
}

/// WriterUserEvent ///
//
WriterUserEvent::WriterUserEvent()
  : initialized_(false), handle_(ops::UserEvent::create(0, true, true))
{
}

void WriterUserEvent::write()
{
  if (!initialized_) {
    initialized_ = true;
    return;
  }
  uint64_t val = 1;
  if (ops::Common::write(handle_, &val, sizeof(val)) < 0)
    throw std::system_error(util::errc(), "WriterUserEvent::write");
}

/// ReaderUserEvent ///
//
void ReaderUserEvent::read()
{
  uint64_t val;
  if (ops::Common::read(handle_, &val, sizeof(val)) < 0)
    throw std::system_error(util::errc(), "ReaderUserEvent::read");
}

} } // namespace ku::fusion

