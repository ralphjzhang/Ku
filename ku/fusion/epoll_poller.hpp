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
#include <functional>
#include "notice.hpp"
#include "notice_board.hpp"
#include "poll_loop.hpp"

namespace ku { namespace fusion { namespace epoll {

class Poller;

class Events : public NoticeBoard
             , private util::noncopyable
{
  friend class Poller;
  // NoticeMap is only used when add/remove/modify notice, shouldn't be the bottleneck,
  // consider stable_vector if really necessary
  typedef std::map<NoticeId, Notice> NoticeMap;

public:
  typedef std::function<bool(std::error_code)> OnError;

  Events(size_t capacity = 16);
  virtual ~Events() { }

  void set_poller(Poller* poller) { poller_ = poller; }
  epoll_event const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }

  Notice* find_notice(epoll_event const& ev);

  using NoticeBoard::apply_updates;

  void set_on_error(OnError const& on_error) { on_error_ = on_error; }

private:
  Poller& poller() { return *poller_; }

  virtual bool add_notice_internal(Notice&& notice);
  virtual bool remove_notice_internal(NoticeId id);
  virtual bool modify_notice_internal(NoticeId id, Notice const& notice);
  virtual Notice* find_notice(NoticeId id);

  epoll_event* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void resize(size_t size) { events_.resize(size); }

private:
  Poller* poller_;
  std::vector<epoll_event> events_;
  unsigned active_count_;
  NoticeMap notices_;
  OnError on_error_;
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
  Poller(Poller&& h) : raw_handle_(h.raw_handle_) { h.clear(); }
  ~Poller() { close(); }
  explicit Poller(int flags);

  Events& poll(Events& evts, std::chrono::milliseconds const& timeout);
  void close();

private:
  int raw_handle() const { return raw_handle_; }
  void clear() { raw_handle_ = 0; }

  int raw_handle_;
};

void translate_events(epoll_event const& ev, Notice& notice);


// =======================================================================================
// epoll::PollLoop is the main loop using epoll
// =======================================================================================
class PollLoop : public fusion::PollLoop
{
public:
  PollLoop() { }
  PollLoop(PollLoop const&) = default;

  void set_on_error(Events::OnError const& on_error) { events_.set_on_error(on_error); }
  virtual NoticeBoard& notices() { return events_; }

private:
  virtual bool loop(std::chrono::milliseconds timeout);

private:
  Events events_;
};

} } } // namespace ku::fusion::epoll


