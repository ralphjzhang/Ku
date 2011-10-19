#pragma once
#include "call_traits.hpp"

namespace ku { namespace net {

addrinfo aif()
{
  // TODO: model this better
  addrinfo addr;
  addr.ai_family = AF_INET;
  addr.ai_socktype = SOCK_STREAM;
  addr.ai_protocol = IPPROTO_TCP;
  addr.ai_flags = AI_PASSIVE;
  return addr;
}

template <typename AcceptHandler>
void handle_channel(Channel& chan, ChannelHub& hub, AcceptHandler handler)
{
  if (chan.has_event(Channel::Read)) {
    if (chan.type() == Channel::Timer) {
      util::if_handle_timer(handler, chan);
      int64_t tick;
      read(chan, &tick, sizeof(tick));
    } else if (chan.type() == Channel::Acceptor) {
      while (true) {
        Address addr;
        Channel conn_ch(accept(chan, addr));
        if (conn_ch.error()) {
          if (conn_ch.error() == std::errc::operation_would_block ||
              conn_ch.error() == std::errc::resource_unavailable_try_again) {
            break;
          } else {
            ;// handler.handle_error(conn_ch);
            // break;
          }
        } else if (util::if_handle_accept(handler, conn_ch, addr))
          hub.adopt_channel(std::move(conn_ch));
      }
    } else {
      assert(chan.type() == Channel::Connection);
      util::if_handle_read(handler, chan);
    }
  }
  if (chan.has_event(Channel::Write))
    util::if_handle_write(handler, chan);
  if (chan.has_event(Channel::Error))
    util::if_handle_error(handler, chan);
  if (chan.has_event(Channel::Close))
    util::if_handle_close(handler, chan, hub);
}

template <typename PollType, typename Handler>
std::error_code server_loop(Address const& addr, Handler eh)
{
  Channel chan;
  chan.set_event_type(Channel::In);
  {
    // Prevent sock referenced out of scope accidentally
    Socket sock = Socket::create(aif());
    sock.listen(addr);
    if (sock.error())
      return sock.error();
    chan.adopt(std::move(sock));
  }

  Channel tchan;
  tchan.set_event_type(Channel::In);
  {
    Timer timer = Timer::create();
    timer.set_interval(std::chrono::seconds(2));
    tchan.adopt(std::move(timer));
  }

  auto poller = PollType::Poller::create();
  auto events = make_events(poller);
  events.adopt_channel(std::move(chan));
  events.adopt_channel(std::move(tchan));

  while (1) {
    poller.poll(events, std::chrono::milliseconds(3000));
    if (poller.error())
      return poller.error();
    using namespace std::placeholders;
    dispatch(events, std::bind(handle_channel<Handler>, _1, _2, eh));
  }
}


} } // namespace ku::net

