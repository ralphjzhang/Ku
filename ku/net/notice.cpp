/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "endpoint.hpp"
#include "notice.hpp"

namespace ku { namespace net {

std::atomic<NoticeId> Notice::next_notice_id(0);

Notice& Notice::operator = (Notice&& notice)
{
  if (this != &notice)
  {
    event_handler_ = notice.event_handler_;
    raw_handle_ = notice.raw_handle_;
    id_ = notice.id_;
    event_types_ = notice.event_types_;
    events_ = notice.events_;
    notice.clear();
  }
  return *this;
}

void Notice::clear()
{
  event_handler_ = nullptr;
  raw_handle_ = 0;
  id_ = 0;
  events_.reset();
  event_types_.reset();
}

std::string to_str(Notice::Events evts)
{
  std::string s;
  if (evts.test(Notice::Close))
    s.append("Close");
  if (evts.test(Notice::Read))
    s.empty() ? s.append("Read") : s.append("|Read");
  if (evts.test(Notice::Write))
    s.empty() ? s.append("Write") : s.append("|Write");
  if (evts.test(Notice::Error))
    s.empty() ? s.append("Error") : s.append("|Error");
  return s;
}

std::string to_str(Notice::EventTypes et)
{
  std::string s;
  if (et.test(Notice::Inbound))
    s.append("Inbound");
  if (et.test(Notice::Outbound))
    s.empty() ? s.append("Outbound") : s.append("|Outbound");
  return s;
}

} } // namespace ku::net

