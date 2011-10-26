#pragma once
#include <sys/epoll.h>
#include <system_error>
#include <map>
#include <vector>
#include <chrono>
#include "channel.hpp"
#include "channel_hub.hpp"

namespace ku { namespace net { namespace epoll {

class Poller;

class Events : public ChannelHub
             , private util::noncopyable
{
  friend class Poller;
  typedef std::map<ChannelId, Channel> ChannelMap;

public:
  Events(Poller& poller, size_t capacity = 16);
  virtual ~Events() { }

  epoll_event const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }

  virtual bool add_channel(Channel&& chan);
  virtual bool remove_channel(Channel const& chan);
  virtual bool modify_channel(Channel const& chan);
  Channel* find_channel(ChannelId id);
  Channel* find_channel(epoll_event const& ev);

private:
  epoll_event* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void resize(size_t size) { events_.resize(size); }

  Poller& poller_;
  std::vector<epoll_event> events_;
  unsigned active_count_;
  ChannelMap channels_;
};


/**
 * epoll::Poller manages epoll file descriptor and associated operation, namely, 
 * epoll_create, epoll_wait
 * It is supposed to be used with epoll::Events
 **/
class Poller : private util::noncopyable
{
  friend class Events;

public:
  Poller(Poller&& h);
  ~Poller() { close(); }

  explicit Poller(int flags);

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void clear_error() { error_.clear(); }

  void close();

private:
  int raw_handle() const { return raw_handle_; }
  void clear() { raw_handle_ = 0; error_.clear(); }

  int raw_handle_;
  std::error_code error_;
};

void translate_events(epoll_event const& ev, Channel& ch);

template <typename Dispatcher>
std::error_code poll_loop(Dispatcher& dispatcher,
    std::chrono::milliseconds timeout = std::chrono::milliseconds(3000))
{
  Poller poller(EPOLL_CLOEXEC);
  if (poller.error())
    return poller.error();

  Events events(poller);
  if (!dispatcher.initialize(events))
    return std::error_code(); //TODO how to populate this error?

  while (!dispatcher.get_quit()) {
    poller.poll(events, timeout);
    if (poller.error()) {
      if (dispatcher.on_error(poller.error()))
        poller.clear_error();
      else
        return poller.error();
    }

    for (unsigned i = 0; i < events.active_count(); ++i) {
      epoll_event const& ev = events.raw_event(i);
      Channel* ch = events.find_channel(ev);
      translate_events(ev, *ch);
      dispatcher.dispatch(*ch, events);
      if (poller.error()) {
        if (dispatcher.on_error(poller.error())) // channel operation may have error
          poller.clear_error();
        else
          return poller.error();
      }
    }
  }
  return std::error_code();
}

} // namespace ku::net::epoll

struct EpollLoop
{
  template <typename Dispatcher>
  std::error_code operator () (Dispatcher& dispatcher,
      std::chrono::milliseconds timeout = std::chrono::milliseconds(3000))
  { return epoll::poll_loop<Dispatcher>(dispatcher, timeout); }
};

} } // namespace ku::net


