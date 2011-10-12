#include <cassert>
#include <sys/epoll.h>
#include <strings.h>
#include "channel.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net { namespace epoll {

// Helper function
int translate_event_types(Channel const& ch)
{
  int event_types = 0;
  if (ch.has_event_type(Channel::In) || ch.has_event_type(Channel::Listen))
    event_types |= (EPOLLIN | EPOLLPRI);
  if (ch.has_event_type(Channel::Out))
    event_types |= EPOLLOUT;
  return event_types;
}

void translate_event_types(int event_types, Channel& ch)
{
  if (event_types & (EPOLLIN | EPOLLPRI))
    ch.set_event_type(Channel::In);
  if (event_types & EPOLLOUT)
    ch.set_event_type(Channel::Out);
}

void translate_events(epoll_event const& ev, Channel& ch)
{
  if ((ev.events & EPOLLHUP) && !(ev.events & EPOLLIN))
    ch.set_event(Channel::Close);
  if (ev.events & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    ch.set_event(Channel::Read);
  if (ev.events & EPOLLOUT)
    ch.set_event(Channel::Write);
  if (ev.events & EPOLLERR)
    ch.set_event(Channel::Error);
}

//////////////
/// Events ///
//////////////

Events::Events(Poller const& p)
  : poller_handle_(p.raw_handle()), events_(Events::InitialCapacity)
{
  clear();
}

Events::Events(Poller const& p, size_t capacity)
  : poller_handle_(p.raw_handle()), events_(capacity)
{
  clear();
}

Events::Events(Events&& e)
{
  poller_handle_ = e.poller_handle_;
  active_count_ = e.active_count_;
  events_ = std::move(e.events_);
  channels_ = std::move(e.channels_);
  e.poller_handle_ = 0;
  e.active_count_ = 0;
  e.events_.clear();
  e.channels_.clear();
}

void Events::clear()
{
  active_count_ = 0;
  ::bzero(raw_begin(), sizeof(epoll_event) * events_.size());
}

bool Events::adopt_channel(Channel&& ch)
{
  auto res = channels_.insert(std::make_pair(ch.raw_handle(), std::move(ch)));
  if (res.second) {
    Channel *ch_ptr = &(res.first->second);
    epoll_event ev;
    ev.data.ptr = ch_ptr;
    ev.events = translate_event_types(*ch_ptr);
    if (::epoll_ctl(poller_handle_, EPOLL_CTL_ADD, ch_ptr->raw_handle(), &ev) != -1)
      return true;
    else
      channels_.erase(res.first);
    // TODO else propogate error?
  }
  return false;
}

Channel* Events::find_channel(int fd)
{
  auto find = channels_.find(fd);
  return channels_.end() == find ? nullptr : &find->second;
}

Channel* Events::find_channel(epoll_event const& ev)
{
  Channel* ch = static_cast<Channel*>(ev.data.ptr);
  assert(ch == &channels_[ch->raw_handle()]);
  return ch;
}

bool Events::remove_channel(int fd)
{
  if (channels_.erase(fd)) {
    epoll_event ev;
    if (::epoll_ctl(poller_handle_, EPOLL_CTL_DEL, fd, &ev) != -1)
      return true;
    // TODO else propogate error?
  }
  return false;
}

bool Events::modify_channel(int fd, int event_types)
{
  auto find = channels_.find(fd);
  if (channels_.end() != find) {
    Channel& ch = find->second;
    assert(ch.raw_handle() == fd);
    epoll_event ev;
    ev.events = event_types;
    if (::epoll_ctl(poller_handle_, EPOLL_CTL_MOD, fd, &ev) != -1) {
      translate_event_types(event_types, ch);
      return true;
    }
    // TODO else propogate error?
  }
  return false;
}

//////////////
/// Poller ///
//////////////

Poller::Poller(Poller&& h) 
  : raw_handle_(h.raw_handle_), error_(h.error_)
{
  h.clear();
}

Poller Poller::create(int flags)
{
  int epoll_fd = epoll_create1(flags);
  if (epoll_fd == -1)
    return Poller(epoll_fd, errno);
  return Poller(epoll_fd, 0);
}

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::epoll_wait(raw_handle(), evts.raw_begin(), evts.events_.size(),
                               timeout.count());
  if (event_num == -1) {
    evts.set_active_count(0);
    set_error(errno);
  }
  evts.set_active_count(event_num);
  if (evts.active_count() >= evts.events_.size())
    evts.resize(evts.active_count() + evts.active_count() / 2);
  return evts;
}

void Poller::close()
{
  if (raw_handle_) {
    if (::close(raw_handle_) == -1)
      set_error(errno);
    else
      clear();
  }
}


} } } // namespace ku::net::poller

