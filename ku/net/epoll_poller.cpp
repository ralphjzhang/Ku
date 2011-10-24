#include <cassert>
#include <sys/epoll.h>
#include "channel.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net { namespace epoll {

// Helper function
int translate_event_types(Channel const& chan)
{
  int event_types = 0;
  if (chan.has_event_type(Channel::In))
    event_types |= (EPOLLIN | EPOLLPRI | EPOLLRDHUP);
  if (chan.has_event_type(Channel::Out))
    event_types |= EPOLLOUT;
  return event_types;
}

void translate_events(epoll_event const& ev, Channel& chan)
{
  if (ev.events & (EPOLLHUP | EPOLLRDHUP))
    chan.set_event(Channel::Close);
  if (ev.events & (EPOLLIN | EPOLLPRI))
    chan.set_event(Channel::Read);
  if (ev.events & EPOLLOUT)
    chan.set_event(Channel::Write);
  if (ev.events & EPOLLERR)
    chan.set_event(Channel::Error);
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
  channels_.clear();
}

bool Events::add_channel(Channel&& chan)
{
  assert(chan.any_event_type());
  auto res = channels_.insert(std::make_pair(chan.id(), std::move(chan)));
  if (res.second) {
    Channel *ch_ptr = &(res.first->second);
    epoll_event ev;
    ev.data.ptr = ch_ptr;
    ev.events = translate_event_types(*ch_ptr);
    if (::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_ADD, ch_ptr->raw_handle(), &ev) == 0)
      return true;
    poller_.set_error(errno);
    channels_.erase(res.first);
  }
  return false;
}

Channel* Events::find_channel(ChannelId id)
{
  auto find = channels_.find(id);
  return channels_.end() == find ? nullptr : &find->second;
}

Channel* Events::find_channel(epoll_event const& ev)
{
  Channel* chan = static_cast<Channel*>(ev.data.ptr);
  assert(chan == &channels_[chan->id()]);
  return chan;
}

bool Events::remove_channel(Channel const& chan)
{
  int raw_handle = chan.raw_handle();
  if (channels_.erase(chan.id())) {
    int ret = ::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_DEL, raw_handle, nullptr);
    // If owner closes the file descriptor, epoll_ctl returns EBADF, which can be ignored
    // If EBADF is because of poller not valid, we will see the error next round anyway
    if (ret == 0 || errno == EBADF)
      return true;
    poller_.set_error(errno);
  }
  return false;
}

bool Events::modify_channel(Channel const& chan)
{
  auto find = channels_.find(chan.id());
  if (channels_.end() != find) {
    Channel& chan_find = find->second;
    assert(chan_find.raw_handle() == chan.raw_handle());
    epoll_event ev;
    ev.events = translate_event_types(chan);
    if (::epoll_ctl(poller_.raw_handle(), EPOLL_CTL_MOD, chan.raw_handle(), &ev) == 0) {
      chan_find.set_event_types(chan.event_types());
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

