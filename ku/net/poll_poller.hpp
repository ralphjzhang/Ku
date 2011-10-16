#pragma once
#include <poll.h>
#include <vector>
#include <utility>
#include <system_error>
#include <chrono>
#include <unordered_map>
#include <ku/util/noncopyable.hpp>
#include "channel.hpp"
#include "channel_hub.hpp"

namespace ku { namespace net {

namespace poll {

class Events : public ChannelHub
             , private ku::util::noncopyable
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

  virtual bool adopt_channel(Channel&& ch);
  virtual bool remove_channel(int fd);
  virtual bool modify_channel(int fd, int event_types);
  Channel* find_channel(int fd);

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

inline Events make_events(Poller& poller) { return Events(); }

void translate_events(pollfd const& ev, Channel& ch);

/**
 * Default event dispatcher for poll::Events
 * It's by design a free template with both type templated, so that user has multiple choices
 * overriding this function, e.g.,
 *
 * template <typename EventHandler>
 * unsigned dispatcher(poll::Events& evts, EventHandler eh);
 *
 * As this template is resoluted by argument-dependent lookup, a dispatcher defined in
 * calling namespace can also override this one.
 **/
template <typename Events, typename EventHandler>
unsigned dispatch(Events& evts, EventHandler eh)
{
  unsigned count = 0;
  for (unsigned i = 0; i < evts.events_count(); ++i) {
    pollfd const& ev = evts.raw_event(i);
    if (ev.revents == 0)
      continue;
    Channel* ch = evts.find_channel(ev.fd);
    translate_events(ev, *ch);
    ++count;
    eh(*ch, evts);
  }
  return count;
}

} // namespace ku::net::poll


struct Poll
{
  typedef poll::Poller Poller;
  typedef poll::Events Events;
};


} } // namespace ku::net

