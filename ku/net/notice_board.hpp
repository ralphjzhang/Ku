/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <initializer_list>
#include "notice.hpp"

namespace ku { namespace net {

class NoticeBoard
{
public:
  template <typename Handle>
  bool add_notice(Handle const& h, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> event_types)
  {
    Notice notice(h.raw_handle(), event_handler);
    for (auto event_type : event_types)
      notice.set_event_type(event_type);
    return add_notice_internal(std::move(notice));
  }

  bool remove_notice(Notice const& notice) { return remove_notice_internal(notice); }
  bool modify_notice(Notice const& notice);

  NoticeBoard() = default;
  virtual ~NoticeBoard() { };

private:
  virtual bool add_notice_internal(Notice&&) = 0;
  virtual bool remove_notice_internal(Notice const&) = 0;
  virtual bool modify_notice_internal(Notice const&) = 0;
};

} } // namespace ku::net

