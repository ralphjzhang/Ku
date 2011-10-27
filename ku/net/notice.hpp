#pragma once
#include <cassert>
#include <type_traits>
#include <system_error>
#include <vector>
#include <string>
#include <bitset>
#include <atomic>
#include "handle.hpp"

namespace ku { namespace net {

class Endpoint;

typedef uint32_t NoticeId;

/**
 * Notice is the link among handle, events and event handlers.
 * A Notice is movable but not copyable, its life cycle is in general managed by Events
 **/
class Notice : util::noncopyable
{
  struct Events : public std::bitset<4> { };
  struct EventTypes : public std::bitset<2> { };
  friend std::string to_str(Events evts);
  friend std::string to_str(EventTypes evts);

  static std::atomic<NoticeId> next_notice_id;

public:
  enum Type : uint8_t { None, Acceptor, Connection, Timer };
  enum EventType : uint8_t { In, Out };
  enum Event : uint8_t { Close, Read, Write, Error };

  Notice() : event_handler_(nullptr), raw_handle_(0), id_(0), type_(Type::None) { }
  Notice(int raw_handle, Type type)
    : event_handler_(nullptr), raw_handle_(raw_handle), id_(++next_notice_id), type_(type) { }

  Notice& operator = (Notice&& notice);
  Notice(Notice&& notice) { *this = std::move(notice); }

  NoticeId id() const { return id_; }
  int raw_handle() const { return raw_handle_; }

  Type type() const { return type_; }
  EventTypes const& event_types() const { return event_types_; }
  void set_event_types(EventTypes const& evts) { event_types_ = evts; }
  void set_event_type(EventType et) { event_types_.set(et); }
  bool has_event_type(EventType et) const { return event_types_.test(et); }
  bool any_event_type() const { return event_types_.any(); }

  Events const& events() const { return events_; }
  void set_event(Event ev) { events_.set(ev); }
  bool has_event(Event ev) const { return events_.test(ev); }
  bool any_event() const { return events_.any(); }

  void set_event_handler(void* event_handler)
  { event_handler_ = event_handler; }

  // Notice: it's the upper level to manage event_handler's life cycle
  template <typename EventHandler>
  EventHandler& event_handler()
  {
    assert(event_handler_);
    return *static_cast<EventHandler*>(event_handler_);
  }

private:
  void clear();

  void* event_handler_;
  int raw_handle_;
  NoticeId id_;
  Type type_;
  EventTypes event_types_;
  Events events_;
};

std::string to_str(Notice::Events evts);
std::string to_str(Notice::EventTypes et);


} } // namespace ku::net

