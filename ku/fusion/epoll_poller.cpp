/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <cassert>
#include "util.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace fusion { namespace epoll {

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

/// Notices ///

Notices::Notices()
  : poller_(nullptr)
{
}

void Notices::clear()
{
  notices_.clear();
}

Notice* Notices::find_notice(epoll_event const& ev)
{
  Notice* notice = static_cast<Notice*>(ev.data.ptr);
  assert(notice == &notices_[notice->id()]);
  return notice;
}

bool Notices::add_notice_internal(Notice&& notice)
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
    notices_.erase(res.first);
    on_error_ && on_error_(util::errc());
  }
  return false;
}

Notice* Notices::find_notice(NoticeId id)
{
  auto find = notices_.find(id);
  return notices_.end() == find ? nullptr : &find->second;
}

bool Notices::remove_notice_internal(NoticeId id)
{
  if (Notice* notice_ptr = find_notice(id)) {
    int raw_handle = notice_ptr->raw_handle();
    if (notices_.erase(id)) {
      int ret = ::epoll_ctl(poller().raw_handle(), EPOLL_CTL_DEL, raw_handle, nullptr);
      // If owner closes the file descriptor, epoll_ctl returns EBADF, which can be ignored
      if (ret == 0 || errno == EBADF)
        return true;
      on_error_ && on_error_(util::errc());
    }
  }
  return false;
}

bool Notices::modify_notice_internal(NoticeId id, Notice const& notice)
{
  if (Notice* notice_ptr = find_notice(id)) {
    epoll_event ev;
    ev.events = translate_event_types(notice);
    if (::epoll_ctl(poller().raw_handle(), EPOLL_CTL_MOD, notice_ptr->raw_handle(), &ev) == 0) {
      notice_ptr->set_event_handler(notice.event_handler());
      notice_ptr->set_event_types(notice.event_types());
      return true;
    }
    on_error_ && on_error_(util::errc());
  }
  return false;
}

/// Poller ///

Poller::Poller(int flags, size_t capacity)
  : events_(capacity), active_count_(0)
{
  if ((raw_handle_ = epoll_create1(flags)) == -1)
    throw std::system_error(util::errc(), "epoll::Poller::Poller");
}

void Poller::poll(std::chrono::milliseconds const& timeout)
{
  int event_num = ::epoll_wait(raw_handle(), &*events_.begin(), events_.size(),
                               timeout.count());
  if (event_num == -1) { // TODO EINTR
    active_count_ = 0;
    throw std::system_error(util::errc(), "epoll::Poller::poll");
  } else {
    active_count_ = event_num;
    if (active_count_ >= events_.size())
      events_.resize(active_count_ + active_count_ / 2);
  }
}

void Poller::close()
{
  if (raw_handle_ > 0 && (raw_handle_ = ::close(raw_handle_)) == -1)
    throw std::system_error(util::errc(), "epoll::Poller::close");
}

/// PollLoop ///
//
bool PollLoop::loop(std::chrono::milliseconds timeout)
{
  Poller poller(EPOLL_CLOEXEC);
  notices_.set_poller(&poller);

  while (!quit_) {
    notices_.apply_updates();
    poller.poll(timeout);

    for (unsigned i = 0; i < poller.active_count(); ++i) {
      epoll_event const& ev = poller.raw_event(i);
      Notice* notice = notices_.find_notice(ev);
      translate_events(ev, *notice);
      dispatch(*notice, notices_);
    }
  }
  return true;
}

} } } // namespace ku::fusion::poller

