#pragma once
#include <system_error>
#include <iostream>
#include "addrinfo.hpp"
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "channel_hub.hpp"
#include "dispatcher.hpp"

namespace ku { namespace net {

template <typename EventHandler>
class Acceptor
{
public:
  Acceptor(Address const& addr) : address_(addr), quit_(false) { }

  bool handle_accept(Channel const& chan, Address const& addr)
  {
    std::cout << "Connection from: " << to_str(addr)  << ", fd=" << chan.raw_handle()
      << std::endl;
    return true;
  }

  void quit() { quit_ = true; }

  void dispatch(Channel& chan, ChannelHub& hub)
  {
    return ku::net::accept_dispatch<Acceptor<EventHandler>, EventHandler>(chan, hub);
  }

  bool get_quit() const { return quit_; }

  bool initialize(ChannelHub& hub)
  {
    AcceptorSocket socket(address_);
    if (socket.error()) {
      std::cout << "Listener error: " << socket.error().message() << std::endl;
      exit(0);
    }
    Channel chan;
    chan.adopt(std::move(socket));
    chan.set_event_type(Channel::In);
    chan.set_event_handler(std::make_shared<EventHandler>());
    hub.adopt_channel(std::move(chan));
    return true;
  }

  bool on_error(std::error_code ec)
  {
    std::cout << "Poller error: " << ec.message() << std::endl;
    return false;
  }

private:
  Address address_;
  bool quit_;
};

} } // namespace ku::net

