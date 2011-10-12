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
    if (chan.listening()) {
      while (true) {
        Address addr;
        Channel conn_ch(chan.accept(addr));
        if (conn_ch.error() == std::errc::operation_would_block)
          break;
        else if (handler.handle_connect(conn_ch))
          evts.adopt_channel(std::move(conn_ch));
      }
    }
    else {
      handler.handle_read(chan);
    }
  }
};

template <typename Poller, typename Handler>
std::error_code server_loop(Address const& addr, Handler eh)
{
  Channel chan;
  chan.adopt(Socket::create(aif()));
  chan.listen(addr);
  if (chan.error())
    return chan.error();

  Poller poller = Poller::create();
  auto events = make_events(poller);
  events.adopt_channel(std::move(chan));

  while (1) {
    poller.poll(events, std::chrono::milliseconds(3000));
    if (poller.error())
      return poller.error();
    using namespace std::placeholders;
    dispatch(events, 
        std::bind(handle_connect<decltype(events), decltype(eh)>, _1, _2, eh));
  }
}


} } // namespace ku::net

