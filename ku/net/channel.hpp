#pragma once
#include <system_error>
#include <vector>
#include <string>
#include <bitset>
#include "util.hpp"

namespace ku { namespace net {

class Address;

/**
 * Channel is the link among Socket, Events and event handlers.
 * A Channel is movable but not copyable, its life cycle is in general managed by Events
 **/
class Channel : private util::noncopyable
{
  typedef std::bitset<4> Events;
  typedef std::bitset<3> EventTypes;
  friend std::string to_str(Events evts);
  friend std::string to_str(EventTypes evts);

public:
  enum EventType { Listen, In, Out };
  enum Event { Close, Read, Write, Error };

  Channel() { clear(); }
  Channel(EventType et) : raw_handle_(0) { set_event_type(et); }
  Channel& operator = (Channel&& ch);
  Channel(Channel&& ch) { *this = std::move(ch); }

  template <typename Handle> void adopt(Handle&& h);
  int raw_handle() const { return raw_handle_; }
  int release_handle();

  bool listen(Address const& addr);
  bool listening() const { return has_event_type(Channel::Listen); }
  Channel accept(Address& addr);

  EventTypes const& event_types() const { return event_types_; }
  void set_event_type(EventType et) { event_types_.set(et); }
  bool has_event_type(EventType et) const { return event_types_.test(et); }

  Events const& events() const { return events_; }
  void set_event(Event ev) { events_.set(ev); }
  bool has_event(Event ev) const { return events_.test(ev); }
  bool any_event() const { return events_.any(); }
  bool non_event() const { return events_.none(); }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

private:
  Channel(int raw_handle, EventType et, std::error_code err)
    : raw_handle_(raw_handle), error_(err)
  { set_event_type(et); }

  void clear();

  int raw_handle_;
  EventTypes event_types_;
  Events events_;
  std::error_code error_;
};

template <typename Handle>
void Channel::adopt(Handle&& h)
{
  raw_handle_ = h.release_handle();
  error_ = h.error();
  if (h.listening())
    set_event_type(Channel::Listen);
}

std::string to_str(Channel::Events evts);
std::string to_str(Channel::EventTypes et);

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
};

// read(Channel const& ch, buffer)
// write(Channel const& ch, buffer)


} } // namespace ku::net

