/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
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

Events::Events(Poller& poller, size_t capacity)
  : poller_(poller), events_(capacity), active_count_(0)
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
    if (::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_ADD, notice_ptr->raw_handle(), &ev) == 0)
      return true;
    poller_.set_error(errno);
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

bool Events::remove_notice_internal(Notice const& notice)
{
  int raw_handle = notice.raw_handle();
  if (notices_.erase(notice.id())) {
    int ret = ::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_DEL, raw_handle, nullptr);
    // If owner closes the file descriptor, epoll_ctl returns EBADF, which can be ignored
    // If EBADF is because of poller not valid, we will see the error next round anyway
    if (ret == 0 || errno == EBADF)
      return true;
    poller_.set_error(errno);
  }
  return false;
}

bool Events::modify_notice_internal(Notice const& notice)
{
  auto find = notices_.find(notice.id());
  if (notices_.end() != find) {
    Notice& notice_find = find->second;
    assert(notice_find.raw_handle() == notice.raw_handle());
    epoll_event ev;
    ev.events = translate_event_types(notice);
    if (::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_MOD, notice.raw_handle(), &ev) == 0) {
      notice_find.set_event_types(notice.event_types());
      return true;
    }
    poller_.set_error(errno);
  }
  return false;
}

/// Poller ///

Poller::Poller(Poller&& h) 
  : raw_handle_(h.raw_handle_), error_(h.error_)
{
  h.clear();
}

Poller::Poller(int flags)
{
  raw_handle_ = epoll_create1(flags);
  if (raw_handle_ == -1)
    set_error(errno);
}

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::epoll_wait(raw_handle(), evts.raw_events(), evts.events_.size(),
                               timeout.count());
  if (event_num == -1) {
    evts.set_active_count(0);
    set_error(errno);
  } else {
    evts.set_active_count(event_num);
    if (evts.active_count() >= evts.events_.size())
      evts.resize(evts.active_count() + evts.active_count() / 2);
  }
  return evts;
}

void Poller::close()
{
  if (raw_handle_ > 0) {
    if (::close(raw_handle_) == -1)
      set_error(errno);
    else
      clear();
  }
}


} } } // namespace ku::net::poller

