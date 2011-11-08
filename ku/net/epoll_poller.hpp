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

namespace ku { namespace net { namespace epoll {

class Poller;

class Events : public NoticeBoard
             , private util::noncopyable
{
  friend class Poller;
  typedef std::map<NoticeId, Notice> NoticeMap;

public:
  Events(size_t capacity = 16);
  virtual ~Events() { }

  void set_poller(Poller* poller) { poller_ = poller; }
  epoll_event const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }

  Notice* find_notice(NoticeId id);
  Notice* find_notice(epoll_event const& ev);

  using NoticeBoard::apply_updates;

  std::error_code error() const { return error_; }
  void set_error(int err_no) { error_ = (std::make_error_code(static_cast<std::errc>(err_no))); }
  void clear_error() { error_.clear(); }

private:
  Poller& poller() { return *poller_; }

  virtual bool add_notice_internal(Notice&& notice);
  virtual bool remove_notice_internal(NoticeId id);
  virtual bool modify_notice_internal(NoticeId id, Notice const& notice);

  epoll_event* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void resize(size_t size) { events_.resize(size); }

  Poller* poller_;
  std::vector<epoll_event> events_;
  unsigned active_count_;
  NoticeMap notices_;
  std::error_code error_;
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

  Events& poll(Events& evts, std::chrono::milliseconds const& timeout);
  void close();

private:
  int raw_handle() const { return raw_handle_; }
  void clear() { raw_handle_ = 0; }

  int raw_handle_;
};

void translate_events(epoll_event const& ev, Notice& notice);

class PollLoop
{
public:
  typedef std::function<bool(std::error_code)> OnError;

  PollLoop() : quit_(false) { }
  PollLoop(PollLoop const&) = default;

  void quit() { quit_ = true; }

  void set_on_error(OnError const& on_error) { on_error_ = on_error; }

  void dispatch(Notice& notice, NoticeBoard& notice_board);
  bool loop(std::chrono::milliseconds timeout);
  bool operator () (std::chrono::milliseconds timeout = std::chrono::milliseconds(-1))
  { return loop(timeout); }

  NoticeBoard& notices() { return events_; }

private:
  bool quit_;
  Events events_;
  OnError on_error_;
};

} } } // namespace ku::net::epoll


