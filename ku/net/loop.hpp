#pragma once
#include "call_traits.hpp"

namespace ku { namespace net {

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
        } else if (util::if_handle_accept(handler, conn_ch, addr)) {
          hub.adopt_channel(std::move(conn_ch));
        }
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

void setup_channels(ChannelHub& hub, Address const& addr)
{
  Channel chan;
  chan.set_event_type(Channel::In);
  {
    // Prevent sock referenced out of scope accidentally
    Socket sock = Socket::create(AddrInfo::create());
    sock.listen(addr);
    if (sock.error()) {
      std::cout << sock.error().message() << std::endl;
      exit(0);
    }
    chan.adopt(std::move(sock));
  }

  Channel tchan;
  tchan.set_event_type(Channel::In);
  {
    Timer timer = Timer::create();
    timer.set_interval(std::chrono::seconds(2));
    tchan.adopt(std::move(timer));
  }

  hub.adopt_channel(std::move(chan));
  hub.adopt_channel(std::move(tchan));
}

} } // namespace ku::net

