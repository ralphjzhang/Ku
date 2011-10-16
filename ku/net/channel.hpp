#pragma once
#include <system_error>
#include <vector>
#include <string>
#include <bitset>
#include "handle.hpp"

namespace ku { namespace net {

class Address;

/**
 * Channel is the link among Socket, Events and event handlers.
 * A Channel is movable but not copyable, its life cycle is in general managed by Events
 **/
class Channel
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

  template <typename Handle> void adopt(Handle&& h);

  int raw_handle() const { return handle_.raw_handle(); }
  bool owner() const { return handle_.owner(); }
  std::error_code error() const { return handle_.error(); }
  template <typename Err> void set_error(Err err) { handle_.set_error(err); }

  Type type() const { return type_; }
  EventTypes const& event_types() const { return event_types_; }
  void set_event_type(EventType et) { event_types_.set(et); }
  bool has_event_type(EventType et) const { return event_types_.test(et); }
  bool any_event_type() const { return event_types_.any(); }

  Events const& events() const { return events_; }
  void set_event(Event ev) { events_.set(ev); }
  bool has_event(Event ev) const { return events_.test(ev); }
  bool any_event() const { return events_.any(); }

private:
  void clear();

  Handle handle_;
  Type type_;
  EventTypes event_types_;
  Events events_;
};

template <typename Handle>
void Channel::adopt(Handle&& h)
{
  type_ = channel_type(h);
  handle_.adopt(std::move(h));
}

std::string to_str(Channel::Events evts);
std::string to_str(Channel::EventTypes et);

/*
class ChannelList : private util::noncopyable
{
  typedef std::vector<Channel> Container;

public:
  ChannelList() = default;
  ~ChannelList() = default;

  ChannelList(ChannelList&& chs);
  ChannelList(size_t capacity) : channels_(capacity) { }

  void add_channel(Channel&& ch) { channels_.push_back(std::move(ch)); }

private:
  Container channels_;
};*/


} } // namespace ku::net

