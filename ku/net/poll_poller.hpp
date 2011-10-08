#pragma once
#include <poll.h>
#include <vector>
#include <utility>
#include <system_error>
#include <chrono>
#include <unordered_map>
#include <ku/util/noncopyable.hpp>
#include "channel.hpp"

namespace ku { namespace net {

class ChannelList;

namespace poll {

enum class EventsType
{
  None = 0, Read = POLLIN | POLLPRI, Write = POLLOUT
};

inline int to_int(EventsType et) { return static_cast<int>(et); }

class Events;

/**
 * poll::Poller wraps system call ::poll
 * It's supposed to be used with poll::Events
 **/
class Poller : private ku::util::noncopyable
{
private:
  Poller() = default;

public:
  Poller(Poller&& h) : error_(h.error_) { h.clear(); }
  ~Poller() = default;

  static Poller create() { return Poller(); }

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { error_.clear(); }
  void close() { clear(); }

private: 
  std::error_code error_;
};

class Events : private ku::util::noncopyable
{
  friend class Poller;
  static const size_t InitialCapacity = 16;

  typedef std::vector<pollfd> EventList;
  // Mapping (file descriptor) --> (Channel, index in events_)
  typedef std::unordered_map<int, std::pair<Channel, size_t> > ChannelMap;

public:
  Events() : events_(Events::InitialCapacity) { clear(); }
  Events(size_t capacity) : events_(capacity) { clear(); }
  Events(Events&& e);

  pollfd const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }
  unsigned events_count() const { return channels_.size(); }

  bool add_channel(Channel&& ch);
  Channel* find_channel(int fd);
  bool remove_channel(int fd);
  bool modify_channel(int fd, int events_type);

private:
  pollfd* raw_begin() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void compress(size_t idx);
  void resize(size_t size) { events_.resize(size); }

  unsigned active_count_;
  EventList events_;
  ChannelMap channels_;
};

void translate_events(pollfd const& ev, Channel& ch);

template <typename EventHandler>
ChannelList& dispatch(Events& evts, ChannelList& chs, EventHandler eh)
{
  for (unsigned i = 0; i < evts.events_count(); ++i) {
    pollfd const& ev = evts.raw_event(i);
    if (ev.revents == 0)
      continue;
    Channel* ch = evts.find_channel(ev.fd);
    translate_events(ev, *ch);
    chs.add(ch);
    eh(*ch);
  }
  return chs;
}


} // namespace ku::net::poll

} } // namespace ku::net

