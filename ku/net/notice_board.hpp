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
  bool add_notice_internal(Notice&& notice);
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
  struct Func {
    Func(int raw_handle, Notice::EventHandler const& event_handler, NoticeBoard* notice_board)
      : notice(raw_handle, event_handler), notice_board(notice_board) { }
    Func(Func const&) = default;
    bool operator()() { return notice_board->add_notice(std::move(notice)); }
    Notice notice;
    NoticeBoard* notice_board;
  };
  Func add_func(h.raw_handle(), event_handler, this);
  for (auto event_type : event_types)
    add_func.notice.set_event_type(event_type);
  NoticeId notice_id = add_func.notice.id();
  std::lock_guard<std::mutex> lock(mutex_);
  notice_update_list_.push_back(add_func);
  pending_updates_ = true;
  return notice_id;
}


} } // namespace ku::net

