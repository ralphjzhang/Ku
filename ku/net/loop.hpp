#pragma once
#include <functional>

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

template <typename Events, typename Handler>
void handle_connect(Channel& chan, Events& evts, Handler handler)
{
  if (chan.has_event(Channel::Read)) {
    if (chan.type() == Channel::Timer) {
      handler.handle_timer(chan);
    }
    else if (chan.type() == Channel::Acceptor) {
      while (true) {
        Address addr;
        Channel conn_ch(accept(chan, addr));
        if (conn_ch.error() == std::errc::operation_would_block) // TODO EAGAIN
          break;
        else if (handler.handle_connect(conn_ch, addr))
          evts.adopt_channel(std::move(conn_ch));
      }
    }
    else {
      handler.handle_read(chan);
    }
  }
};

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
    dispatch(events, 
        std::bind(handle_connect<decltype(events), Handler>, _1, _2, eh));
  }
}


} } // namespace ku::net

