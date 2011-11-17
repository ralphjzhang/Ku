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

/// PublisherUserEvent ///
//
PublisherUserEvent::PublisherUserEvent()
  : initialized_(false), handle_(ops::UserEvent::create(0, true, true)), subscribers_(0)
{
}

bool PublisherUserEvent::try_publish()
{
  if (!initialized_) {
    initialized_ = true;
    return false;
  }

  if (ops::Common::write(handle_, &subscribers_, sizeof(subscribers_)) < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return false;
    else
      throw std::system_error(util::errc(), "PublisherUserEvent::try_publish");
  }
  return true;
}

/// SubscriberUserEvent ///
//
bool SubscriberUserEvent::try_read()
{
  uint64_t val;
  if (ops::Common::read(publisher_.handle_, &val, sizeof(val)) < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      return false;
    else
      throw std::system_error(util::errc(), "SubscriberUserEvent::try_read");
  }
  assert(val == 1);
  return true;
}

} } // namespace ku::fusion

