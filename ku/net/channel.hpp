#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include <system_error>
#include <vector>
#include <string>
#include <bitset>
#include "handle.hpp"

namespace ku { namespace net {

class Address;

/**
 * Channel is the link among handle, events and event handlers.
 * A Channel is movable but not copyable, its life cycle is in general managed by Events
 **/
class Channel : util::noncopyable
{
  struct Events : public std::bitset<4> { };
  struct EventTypes : public std::bitset<2> { };
  friend std::string to_str(Events evts);
  friend std::string to_str(EventTypes evts);

  friend Channel accept(Channel& chan, Address& addr);

public:
  enum Type : uint8_t { None, Acceptor, Connection, Timer };
  enum EventType : uint8_t { In, Out };
  enum Event : uint8_t { Close, Read, Write, Error };

  Channel() : handle_(0, false), type_(Type::None) { }
  Channel& operator = (Channel&& chan);
  Channel(Channel&& chan) { *this = std::move(chan); }

  template <typename Handle>
  typename std::enable_if<!std::is_lvalue_reference<Handle>::value, void>::type
  adopt(Handle&& h)
  {
    type_ = channel_type(h);
    handle_.adopt(std::move(h));
  }

  Handle& handle() { return handle_; }
  Handle const& handle() const { return handle_; }
  int raw_handle() const { return handle_.raw_handle(); }
  bool owner() const { return handle_.owner(); }
  std::error_code error() const { return handle_.error(); }
  template <typename Err> void set_error(Err err) { handle_.set_error(err); }

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

  template <typename EventHandler>
  void set_event_handler(std::shared_ptr<EventHandler> eh_ptr)
  { event_handler_ = eh_ptr; }

  template <typename EventHandler>
  EventHandler& event_handler()
  {
    assert(event_handler_.get());
    return *static_cast<EventHandler*>(event_handler_.get());
  }

private:
  void clear();

  Handle handle_;
  Type type_;
  EventTypes event_types_;
  Events events_;
  std::shared_ptr<void> event_handler_;
};

std::string to_str(Channel::Events evts);
std::string to_str(Channel::EventTypes et);


} } // namespace ku::net

