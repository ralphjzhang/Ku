/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include "util.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net { namespace epoll {

// Helper functions
int translate_event_types(Notice const& notice)
{
  int event_types = 0;
  if (notice.has_event_type(Notice::Inbound))
    event_types |= (EPOLLIN | EPOLLPRI | EPOLLRDHUP);
  if (notice.has_event_type(Notice::Outbound))
    event_types |= EPOLLOUT;
  if (notice.has_event_type(Notice::Edge))
    event_types |= EPOLLET;
  return event_types;
}

void translate_events(epoll_event const& ev, Notice& notice)
{
  if (ev.events & (EPOLLHUP | EPOLLRDHUP))
    notice.set_event(Notice::Close);
  if (ev.events & (EPOLLIN | EPOLLPRI))
    notice.set_event(Notice::Read);
  if (ev.events & EPOLLOUT)
    notice.set_event(Notice::Write);
  if (ev.events & EPOLLERR)
    notice.set_event(Notice::Error);
}

/// Events ///

Events::Events(size_t capacity)
  : poller_(nullptr), events_(capacity), active_count_(0)
{
}

void Events::clear()
{
  events_.clear();
  active_count_ = 0;
  notices_.clear();
}

bool Events::add_notice_internal(Notice&& notice)
{
  assert(notice.any_event_type());
  auto res = notices_.insert(std::make_pair(notice.id(), std::move(notice)));
  if (res.second) {
    Notice *notice_ptr = &(res.first->second);
    epoll_event ev;
    ev.data.ptr = notice_ptr;
    ev.events = translate_event_types(*notice_ptr);
    if (::epoll_ctl(poller().raw_handle(), EPOLL_CTL_ADD, notice_ptr->raw_handle(), &ev) == 0)
      return true;
    set_error(errno);
    notices_.erase(res.first);
  }
  return false;
}

Notice* Events::find_notice(NoticeId id)
{
  auto find = notices_.find(id);
  return notices_.end() == find ? nullptr : &find->second;
}

Notice* Events::find_notice(epoll_event const& ev)
{
  Notice* notice = static_cast<Notice*>(ev.data.ptr);
  assert(notice == &notices_[notice->id()]);
  return notice;
}

bool Events::remove_notice_internal(NoticeId id)
{
  if (Notice* notice_ptr = find_notice(id)) {
    int raw_handle = notice_ptr->raw_handle();
    if (notices_.erase(id)) {
      int ret = ::epoll_ctl(poller().raw_handle(), EPOLL_CTL_DEL, raw_handle, nullptr);
      // If owner closes the file descriptor, epoll_ctl returns EBADF, which can be ignored
      if (ret == 0 || errno == EBADF)
        return true;
      set_error(errno);
    }
  }
  return false;
}

bool Events::modify_notice_internal(NoticeId id, Notice const& notice)
{
  if (Notice* notice_ptr = find_notice(id)) {
    epoll_event ev;
    ev.events = translate_event_types(notice);
    if (::epoll_ctl(poller().raw_handle(), EPOLL_CTL_MOD, notice_ptr->raw_handle(), &ev) == 0) {
      notice_ptr->set_event_handler(notice.event_handler());
      notice_ptr->set_event_types(notice.event_types());
      return true;
    }
    set_error(errno);
  }
  return false;
}

/// Poller ///

Poller::Poller(Poller&& h) : raw_handle_(h.raw_handle_)
{
  h.clear();
}

Poller::Poller(int flags)
{
  if ((raw_handle_ = epoll_create1(flags)) == -1)
    throw std::system_error(util::errc(), "epoll::Poller::Poller");
}

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::epoll_wait(raw_handle(), evts.raw_events(), evts.events_.size(),
                               timeout.count());
  if (event_num == -1) {
    evts.set_active_count(0);
    throw std::system_error(util::errc(), "epoll::Poller::poll");
  } else {
    evts.set_active_count(event_num);
    if (evts.active_count() >= evts.events_.size())
      evts.resize(evts.active_count() + evts.active_count() / 2);
  }
  return evts;
}

void Poller::close()
{
  if (raw_handle_ > 0 && (raw_handle_ = ::close(raw_handle_)) == -1)
    throw std::system_error(util::errc(), "epoll::Poller::close");
}

/// PollLoop ///

void PollLoop::dispatch(Notice& notice, NoticeBoard& notice_board)
{
  Notice::EventHandler& event_handler = notice.event_handler();
  // Read
  if (notice.has_event(Notice::Read))
    event_handler(Notice::Read, notice.id());
  // Write
  if (notice.has_event(Notice::Write))
    event_handler(Notice::Write, notice.id());
  // Error
  if (notice.has_event(Notice::Error))
    if (!event_handler(Notice::Error, notice.id()))
      notice_board.remove_notice(notice.id());
  // Close
  if (notice.has_event(Notice::Close)) {
    event_handler(Notice::Close, notice.id());
    notice_board.remove_notice(notice.id());
  }
}

bool PollLoop::loop(std::chrono::milliseconds timeout)
{
  Poller poller(EPOLL_CLOEXEC);
  events_.set_poller(&poller);

  while (!quit_) {
    events_.apply_updates();
    poller.poll(events_, timeout);

    for (unsigned i = 0; i < events_.active_count(); ++i) {
      epoll_event const& ev = events_.raw_event(i);
      Notice* notice = events_.find_notice(ev);
      translate_events(ev, *notice);
      dispatch(*notice, events_);
      // These errors are from add/remove/modify events, can be ignored
      // If user doesn't register error handler, these are ignored TODO review this
      if (events_.error()) {
        if (on_error_ && !on_error_(events_.error())) 
          return false;
        else
          events_.clear_error();
      }
    }
  }
  return true;
}

} } } // namespace ku::net::poller

