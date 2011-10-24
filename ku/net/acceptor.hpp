#pragma once
#include <system_error>
#include <memory>
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
  Acceptor(Address const& addr)
    : address_(addr), quit_(false), acceptor_socket_(addr) { }

  bool handle_accept_error(AcceptorSocket const& sock)
  {
    return true;
  }

  void quit() { quit_ = true; }

  // Acceptor requirement
  void handle_accept(ChannelHub& hub)
  {
    while (true) {
      Address addr;
      StreamSocket conn_socket = acceptor_socket_.accept(addr);
      if (!acceptor_socket_.error()) {
        std::cout << "Connection from: " << to_str(addr) << std::endl;
        Channel conn_chan(conn_socket.raw_handle(), Channel::Connection);
        conn_chan.set_event_type(Channel::In);
        conn_chan.set_event_handler(new EventHandler(std::move(conn_socket)));
        hub.add_channel(std::move(conn_chan));
      } else {
        std::error_code ec = acceptor_socket_.error();
        if (ec == std::errc::operation_would_block ||
            ec == std::errc::resource_unavailable_try_again) {
          // All incoming connections handled
          break;
        } else {
          // Acceptor error
          handle_accept_error(acceptor_socket_);
          break;
        }
      }
    }
  }

  // Dispatcher requirement
  void dispatch(Channel& chan, ChannelHub& hub)
  {
    return ku::net::dispatch<Acceptor<EventHandler>, EventHandler>(chan, hub);
  }

  bool get_quit() const { return quit_; }

  bool initialize(ChannelHub& hub)
  {
    if (acceptor_socket_.error()) {
      std::cout << "Listener error: " << acceptor_socket_.error().message() << std::endl;
      exit(0);
    }
    Channel chan(acceptor_socket_.raw_handle(), Channel::Acceptor);
    chan.set_event_type(Channel::In);
    chan.set_event_handler(this);
    hub.add_channel(std::move(chan));
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
  AcceptorSocket acceptor_socket_;
};

} } // namespace ku::net

