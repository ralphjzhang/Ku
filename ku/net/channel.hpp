#pragma once
#include <strings.h>
#include <system_error>
#include <vector>
#include <string>
#include <bitset>
#include <ku/util/noncopyable.hpp>

namespace ku { namespace net {

class Socket;

/**
 * Channel is the link among Socket, Events and event handlers.
 * Channels don't own socket file descriptors
 * A Channel is movable but not copyable, its life cycle is in general managed by Events
 **/
class Channel : private ku::util::noncopyable
{
  typedef std::bitset<4> Events;
  friend std::string to_str(Events evts);

public:
  enum Event { Close, Read, Write, Error };

  Channel() { clear(); }

  template <typename Handle>
  explicit Channel(Handle const& h) : raw_handle_(h.raw_handle()) { }

  template <typename Handle>
  Channel(Handle const& h, int events_type)
    : raw_handle_(h.raw_handle()), events_type_(events_type)
  { }

  Channel(Channel&& ch) { *this = std::move(ch); }
  Channel& operator = (Channel&& ch);

  int raw_handle() const { return raw_handle_; }

  int events_type() const { return events_type_; }
  void set_events_type(int et) { events_type_ = et; }

  Events const& events() const { return events_; }

  template <Event Ev>
  void set_event() { events_.set(Ev); }

  template <Event Ev>
  bool has_event() { return events_.test(Ev); }

  bool any_event() { return events_.any(); }
  bool non_event() { return events_.none(); }

private:
  void clear() { ::bzero(this, sizeof(Channel)); events_.reset(); }

  int raw_handle_;
  int events_type_;
  Events events_;
};

std::string to_str(Channel::Events evts);

class ChannelList : public std::vector<Channel*>
{
  typedef std::vector<Channel*> Base;

public:
  ChannelList() = default;
  ~ChannelList() = default;

  ChannelList(size_t capacity) : Base(capacity) { }

  void add(Channel* ch) { push_back(ch); }
  Channel& channel(size_t n) { return *at(n); }
  Channel const& channel(size_t n) const { return *at(n); }
};


} } // namespace ku::net

