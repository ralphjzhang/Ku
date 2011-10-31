/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <atomic>
#include <mutex>
#include <initializer_list>
#include <functional>
#include <vector>
#include <memory>
#include "notice.hpp"

namespace ku { namespace net {

class NoticeBoard
{
  typedef std::function<bool()> NoticeUpdate;
  typedef std::vector<NoticeUpdate> NoticeUpdateList;

public:
  NoticeBoard() = default;
  virtual ~NoticeBoard() { };

  template <typename Handle>
  NoticeId add_notice(Handle const& h, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> event_types);

  bool remove_notice(NoticeId notice_id);

  bool modify_notice(NoticeId notice_id, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> event_types);

protected:
  void apply_updates();

private:
  NoticeId add_notice(int raw_handle, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> event_types);

  bool add_notice_internal(std::shared_ptr<Notice> notice_ptr);
  bool remove_notice_internal(NoticeId notice_id);
  bool modify_notice_internal(NoticeId notice_id, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> event_types);

private:
  virtual bool add_notice(Notice&&) = 0;
  virtual bool remove_notice(Notice const&) = 0;
  virtual bool modify_notice(Notice const&) = 0;
  virtual Notice* find_notice(NoticeId notice_id) = 0;

private:
  std::atomic<bool> pending_updates_;
  std::mutex mutex_;
  NoticeUpdateList notice_update_list_;
};


template <typename Handle>
NoticeId NoticeBoard::add_notice(Handle const& h, Notice::EventHandler const& event_handler,
    std::initializer_list<Notice::EventType> event_types)
{
  return add_notice(h.raw_handle(), event_handler, event_types);
}


} } // namespace ku::net

