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

class Address;
class ChannelHub;

template <typename EventHandler>
class Acceptor : public Dispatcher<EventHandler>
{
public:
  Acceptor(Address const& addr) : address_(addr) { }

  bool on_setup(ChannelHub& hub)
  {
    Socket socket(Socket::create(AddrInfo::create()));
    socket.listen(address_);
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
};

} } // namespace ku::net

