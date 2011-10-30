/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "notice_board.hpp"

namespace ku { namespace net {

bool NoticeBoard::remove_notice(NoticeId notice_id)
{
  pending_updates_ = true;
  std::lock_guard<std::mutex> lock(mutex_);
  notice_update_list_.push_back([this, notice_id]() {
      return remove_notice_internal(notice_id);
      });
  return true;
}

bool NoticeBoard::modify_notice(NoticeId notice_id, Notice::EventHandler const& event_handler,
    std::initializer_list<Notice::EventType> event_types)
{
  pending_updates_ = true;
  std::lock_guard<std::mutex> lock(mutex_);
  notice_update_list_.push_back([this, notice_id, &event_handler, &event_types]() {
      return modify_notice_internal(notice_id, event_handler, event_types);
      });
  return true;
}

bool NoticeBoard::add_notice_internal(Notice&& notice)
{
  return add_notice(std::move(notice));
}


bool NoticeBoard::remove_notice_internal(NoticeId notice_id)
{
  if (Notice* notice_ptr = find_notice(notice_id))
    return remove_notice(*notice_ptr);
  return false;
}

bool NoticeBoard::modify_notice_internal(NoticeId notice_id,
    Notice::EventHandler const& event_handler,
    std::initializer_list<Notice::EventType> event_types)
{
  if (Notice* notice_ptr = find_notice(notice_id)) {
    notice_ptr->set_event_handler(event_handler);
    for (auto event_type : event_types)
      notice_ptr->set_event_type(event_type);
    return modify_notice(*notice_ptr);
  }
  return false;
}

void NoticeBoard::apply_updates()
{
  if (!pending_updates_)
    return;
  NoticeUpdateList list;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    list.swap(notice_update_list_);
  }
  for (NoticeUpdate& update : list)
    update();
}

} } // namespace ku::net

