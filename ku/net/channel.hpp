#pragma once
#include <strings.h>
#include <system_error>
#include <vector>
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
public:
  Channel() : raw_handle_(0), events_type_(0), events_(0) { }

  template <typename Handle>
  explicit Channel(Handle const& handle)
    : raw_handle_(handle.raw_handle())
  { }

  template <typename Handle>
  Channel(Handle const& handle, int events_type)
    : raw_handle_(handle.raw_handle()), events_type_(events_type)
  { }

  Channel(Channel&& ch) { *this = std::move(ch); }

  Channel& operator = (Channel&& ch);

  int raw_handle() const { return raw_handle_; }

  void clear() { ::bzero(this, sizeof(Channel)); }

  int events_type() const { return events_type_; }
  void set_events_type(int et) { events_type_ = et; }
  int events() const { return events_; }
  void set_events(int events) { events_ = events; }

  void handle_event();

private:
  int raw_handle_;
  int events_type_;
  int events_;
};


class ChannelList
{
public:
  void add(Channel* ch) { channels_.push_back(ch); }

private:
  std::vector<Channel*> channels_; 
};


} } // namespace ku::net

