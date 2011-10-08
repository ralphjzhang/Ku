#pragma once
#include <sys/epoll.h>
#include <system_error>
#include <map>
#include <vector>
#include <chrono>
#include <ku/util/noncopyable.hpp>
#include "channel.hpp"

namespace ku { namespace net {

namespace epoll {

class Events;

/**
 * epoll::Poller manages epoll file descriptor and associated operation, namely, 
 * epoll_create, epoll_wait
 * It is supposed to be used with epoll::Events
 **/
class Poller : private ku::util::noncopyable
{
private:
  explicit Poller(int raw_handle) : raw_handle_(raw_handle) { }

  template <typename Err>
  Poller(int raw_handle, Err err) : raw_handle_(raw_handle) { set_error(err); }

public:
  Poller(Poller&& h) : raw_handle_(h.raw_handle_), error_(h.error_) { h.clear(); }
  ~Poller() { close(); }

  static Poller create(int flags = EPOLL_CLOEXEC);
  int raw_handle() const { return raw_handle_; }

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void close();

private:
  void clear() { raw_handle_ = 0; error_.clear(); }

  int raw_handle_;
  std::error_code error_;
};


class Events
{
  friend class Poller;

  static const size_t InitialCapacity = 16;
  // Mapping (file descriptor) -> Channel
  typedef std::map<int, Channel> ChannelMap;

public:
  Events(Poller const& p)
    : poller_handle_(p.raw_handle()), events_(Events::InitialCapacity)
  { clear(); }
  Events(Poller const& p, size_t capacity)
    : poller_handle_(p.raw_handle()), events_(capacity)
  { clear(); }

  Events(Events&& e);

  epoll_event const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }

  bool add_channel(Channel&& ch);
  Channel* find_channel(int fd);
  Channel* find_channel(epoll_event const& ev);
  bool remove_channel(int fd);
  bool modify_channel(int fd, int events_type);

private:
  epoll_event* raw_begin() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void resize(size_t size) { events_.resize(size); }

  int poller_handle_;
  std::vector<epoll_event> events_;
  unsigned active_count_;
  ChannelMap channels_;
};

void translate_events(epoll_event const& ev, Channel& ch);

/**
 * Default event dispatcher for epoll::Events
 * It's by design a free template with both type templated, so that user has multiple choices
 * overriding this function, e.g.,
 *
 * template <typename EventHandler>
 * unsigned dispatcher(epoll::Events& evts, EventHandler eh);
 *
 * As this template is resoluted by argument-dependent lookup, a dispatcher defined in
 * calling namespace can also override this one.
 **/
template <typename Events, typename EventHandler>
unsigned dispatch(Events& evts, EventHandler eh)
{
  for (unsigned i = 0; i < evts.active_count(); ++i) {
    epoll_event const& ev = evts.raw_event(i);
    Channel* ch = evts.find_channel(ev);
    translate_events(ev, *ch);
    eh(*ch);
  }
  return evts.active_count();
}

} } } // namespace ku::net::epoll

