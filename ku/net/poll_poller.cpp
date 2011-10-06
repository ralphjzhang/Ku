#include "poll_poller.hpp"
#include "channel.hpp"

namespace ku { namespace net { namespace poll {

void Events::add_channel(Channel& ch)
{
  if (count_ >= size())
    resize(count_ * 2);
  auto & ev = events_[count_];
  ev.fd = ch.raw_handle();
  ev.events = ch.events_type();
  channels_[ev.fd] = &ch;
  ++count_;
}

ChannelList& dispatch(Events& evts, ChannelList& chs)
{
  int current_count = evts.count();
  for (int i = 0; i < current_count; ++i) {
    auto const& ev = evts.raw_event(i);
    if (ev.revents == 0)
      continue;
    // TODO error handling
    auto ch = evts.channels_[ev.fd];
    ch->set_events(ev.events);
    chs.add(ch);
  }
  return chs;
}

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::poll(evts.raw_begin(), evts.count(), timeout.count());
  if (event_num == -1)
    set_error(errno);
  else
    if (event_num >= evts.size())
      evts.resize(event_num * 2);
  evts.set_count(event_num);
  return evts;
}

} } } // namespace ku::net::poll

