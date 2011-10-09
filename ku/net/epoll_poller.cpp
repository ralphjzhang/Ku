#include <cassert>
#include <sys/epoll.h>
#include <strings.h>
#include "channel.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net { namespace epoll {

// Helper function
int translate_events_type(Channel::EventsType et)
{
  int events_type = 0;
  if (et | Channel::In)
    events_type |= (EPOLLIN | EPOLLPRI);
  if (et | Channel::Out)
    events_type |= EPOLLOUT;
  return events_type;
}

//////////////
/// Poller ///
//////////////

Poller Poller::create(int flags)
{
  int epoll_fd = epoll_create1(flags);
  if (epoll_fd == -1)
    return Poller(epoll_fd, errno);
  return Poller(epoll_fd);
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

//////////////
/// Events ///
//////////////

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

bool Events::add_channel(Channel&& ch)
{
  auto res = channels_.insert(std::make_pair(ch.raw_handle(), ch));
  if (res.second) {
    Channel *ch_ptr = &(res.first->second);
    epoll_event ev;
    ev.data.ptr = ch_ptr;
    ev.events = translate_events_type(ch.events_type());
    if (::epoll_ctl(poller_handle_, EPOLL_CTL_ADD, ch.raw_handle(), &ev) != -1)
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

bool Events::modify_channel(int fd, int events_type)
{
  auto find = channels_.find(fd);
  if (channels_.end() != find) {
    Channel& ch = find->second;
    assert(ch.raw_handle() == fd);
    epoll_event ev;
    ev.events = events_type;
    if (::epoll_ctl(poller_handle_, EPOLL_CTL_MOD, fd, &ev) != -1) {
      ch.set_events_type(ch.events_type());
      return true;
    }
    // TODO else propogate error?
  }
  return false;
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

} } } // namespace ku::net::poller

