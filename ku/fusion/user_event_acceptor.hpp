/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "user_event.hpp"
#include "user_event_endpoint.hpp"
#include "notice_board.hpp"

namespace ku { namespace fusion {

class UserEventAcceptor
{
public:
  UserEventAcceptor(UserEventEndpoint const& local_endpoint, NoticeBoard& notices)
    : local_endpoint_(local_endpoint), notices_(notices)
  {
  }

private:
  UserEventEndpoint local_endpoint_;
  NoticeBoard& notices_;
};

} } // namespace ku::fusion

