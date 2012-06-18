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

namespace ku { namespace fusion {

class NoticeBoard
{
  struct Update {
    NoticeId id;
    Notice notice;

    Update() : id(0) { }
    Update(NoticeId id) : id(id) { } // Remove
    Update(NoticeId id, Notice&& notice) : id(id), notice(std::move(notice)) { } // Add/Modify
  };
  using UpdateList = std::vector<Update>;

public:
  NoticeBoard();
  virtual ~NoticeBoard() { };

  template <typename Handle>
  NoticeId add_notice(Handle const& h, std::initializer_list<Notice::EventType> const& event_types,
      Notice::EventHandler const& event_handler);

  bool remove_notice(NoticeId notice_id);

  bool modify_notice(NoticeId notice_id, Notice::EventHandler const& event_handler,
      std::initializer_list<Notice::EventType> const& event_types);

protected:
  void apply_updates();

private:
  NoticeId add_notice(int raw_handle, std::initializer_list<Notice::EventType> const& event_types,
      Notice::EventHandler const& event_handler);

  virtual bool add_notice_internal(Notice&&) = 0;
  virtual bool remove_notice_internal(NoticeId) = 0;
  virtual bool modify_notice_internal(NoticeId, Notice const&) = 0;
  virtual Notice* find_notice(NoticeId notice_id) = 0;

private:
  std::atomic_bool pending_updates_;
  UpdateList update_list_;
  std::mutex mutex_;
};


template <typename Handle>
NoticeId NoticeBoard::add_notice(Handle const& h, std::initializer_list<Notice::EventType> const& event_types,
    Notice::EventHandler const& event_handler)
{
  return add_notice(h.raw_handle(), event_types, event_handler);
}


} } // namespace ku::fusion

