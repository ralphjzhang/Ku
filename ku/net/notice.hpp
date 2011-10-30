/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cassert>
#include <type_traits>
#include <system_error>
#include <functional>
#include <vector>
#include <string>
#include <bitset>
#include <atomic>
#include "handle.hpp"

namespace ku { namespace net {

class Endpoint;

typedef uint32_t NoticeId;

// =======================================================================================
// Notice is the link among handle, events and event handlers.
// NoticeBoard manages the lifecycle of a notice
// =======================================================================================
class Notice
{
  struct Events : public std::bitset<4> { };
  struct EventTypes : public std::bitset<3> { };
  friend std::string to_str(Events evts);
  friend std::string to_str(EventTypes evts);

  static std::atomic<NoticeId> next_notice_id;

public:
  enum EventType : uint8_t { Inbound, Outbound, Edge };
  enum Event : uint8_t { Close, Read, Write, Error };
  typedef std::function<bool(Event, NoticeId)> EventHandler;

  Notice() : raw_handle_(0), id_(0) { }
  Notice(int raw_handle, EventHandler event_handler)
    : raw_handle_(raw_handle), id_(++next_notice_id), event_handler_(event_handler) { }

  Notice& operator = (Notice&& notice);
  Notice(Notice const&) = default;
  Notice(Notice&& notice) { *this = std::move(notice); }

  NoticeId id() const { return id_; }
  int raw_handle() const { return raw_handle_; }

  EventTypes const& event_types() const { return event_types_; }
  void set_event_types(EventTypes const& evts) { event_types_ = evts; }
  void set_event_type(EventType et) { event_types_.set(et); }
  bool has_event_type(EventType et) const { return event_types_.test(et); }
  bool any_event_type() const { return event_types_.any(); }

  Events const& events() const { return events_; }
  void set_event(Event ev) { events_.set(ev); }
  bool has_event(Event ev) const { return events_.test(ev); }
  bool any_event() const { return events_.any(); }

  void set_event_handler(EventHandler const& event_handler)
  { event_handler_ = event_handler; }
  EventHandler& event_handler() { return event_handler_; }

private:
  void clear();

  int raw_handle_;
  NoticeId id_;
  EventTypes event_types_;
  Events events_;
  EventHandler event_handler_;
};

std::string to_str(Notice::Events evts);
std::string to_str(Notice::EventTypes et);


} } // namespace ku::net

