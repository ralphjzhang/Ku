#include <sys/epoll.h>
#include <strings.h>
#include "channel.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net { namespace epoll {

Poller Poller::create(int flags)
{
  int epoll_fd = epoll_create1(flags);
  if (epoll_fd == -1)
    return Poller(epoll_fd, errno);
  return Poller(epoll_fd);
}

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::epoll_wait(raw_handle(), evts.raw_begin(), evts.size(), timeout.count());
  if (event_num == -1)
    set_error(errno);
  else
    if (event_num >= evts.size())
      evts.resize(event_num * 2);
  evts.set_count(event_num);
  return evts;
}

ChannelList& dispatch(Events const& evts, ChannelList& chs)
{
  for (int i = 0; i < evts.count(); ++i) {
    auto const& ev = evts.raw_event(i);
    auto ch = static_cast<Channel*>(ev.data.ptr);
    ch->set_events(ev.events);
    chs.add(ch);
  }
  return chs;
}

Poller& Poller::update(int op, Channel const& ch)
{
  epoll_event event;
  bzero(&event, sizeof(event));
  event.data.fd = ch.raw_handle();
  event.data.ptr = const_cast<Channel*>(&ch);
  event.events = ch.events_type();
  if (::epoll_ctl(raw_handle(), op, ch.raw_handle(), &event) == -1)
    set_error(errno);
  return *this;
}

Poller& Poller::add_channel(Channel const& ch)
{
  return update(EPOLL_CTL_ADD, ch);
}

Poller& Poller::remove_channel(Channel const& ch)
{
  return update(EPOLL_CTL_DEL, ch);
}

Poller& Poller::modify_channel(Channel const& ch)
{
  return update(EPOLL_CTL_MOD, ch);
}

Poller& close(Poller& h)
{
  if (::close(h.raw_handle()) == -1)
    h.set_error(errno);
  return h;
}

} } } // namespace ku::net::poller
