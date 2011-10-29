/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <sys/epoll.h>
#include <system_error>
#include <map>
#include <vector>
#include <chrono>
#include "notice.hpp"
#include "notice_board.hpp"

namespace ku { namespace net { namespace epoll {

class Poller;

class Events : public NoticeBoard
             , private util::noncopyable
{
  friend class Poller;
  typedef std::map<NoticeId, Notice> NoticeMap;

public:
  Events(Poller& poller, size_t capacity = 16);
  virtual ~Events() { }

  epoll_event const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }

  Notice* find_notice(NoticeId id);
  Notice* find_notice(epoll_event const& ev);

private:
  virtual bool add_notice_internal(Notice&& notice);
  virtual bool remove_notice_internal(Notice const& notice);
  virtual bool modify_notice_internal(Notice const& notice);

  epoll_event* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void resize(size_t size) { events_.resize(size); }

  Poller& poller_;
  std::vector<epoll_event> events_;
  unsigned active_count_;
  NoticeMap notices_;
};

// =======================================================================================
// epoll::Poller manages epoll file descriptor and associated operation, namely, 
// epoll_create, epoll_wait
// It is supposed to be used with epoll::Events
// =======================================================================================
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

void translate_events(epoll_event const& ev, Notice& notice);

template <typename Dispatcher>
class PollLoop
{
public:
  PollLoop() : quit_(false) { }
  PollLoop(PollLoop const&) = default;

  void quit() { quit_ = true; }
  std::error_code error() const { return error_; }

  bool operator () (Dispatcher& dispatcher,
      std::chrono::milliseconds timeout = std::chrono::milliseconds(3000))
  {
    Poller poller(EPOLL_CLOEXEC);
    if (poller.error()) {
      error_ = poller.error();
      return false;
    }

    Events events(poller);
    if (!dispatcher.initialize(events))
      return false;

    while (!quit_) {
      poller.poll(events, timeout);
      if (poller.error()) {
        error_ = poller.error();
        return false;
      }

      for (unsigned i = 0; i < events.active_count(); ++i) {
        epoll_event const& ev = events.raw_event(i);
        Notice* notice = events.find_notice(ev);
        translate_events(ev, *notice);
        dispatcher.dispatch(*notice, events);
        if (poller.error()) {
          error_ = poller.error(); // TODO this error might be ignorable, like remove_notice..
          return false;
        }
      }
    }
    return true;
  }

private:
  bool quit_;
  std::error_code error_;
};

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
      Notice* notice = events.find_notice(ev);
      translate_events(ev, *notice);
      dispatcher.dispatch(*notice, events);
      if (poller.error()) {
        if (dispatcher.on_error(poller.error())) // notice operation may have error
          poller.clear_error();
        else
          return poller.error();
      }
    }
  }
  return std::error_code();
}

} } } // namespace ku::net::epoll


