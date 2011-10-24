#pragma once
#include <iostream>
#include <cstring>
#include "address.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class TCPConnection
{
public:
  TCPConnection(StreamSocket&& socket, Address const& peer_address) 
    : socket_(std::move(socket)), peer_address_(peer_address)
  { }
  virtual ~TCPConnection() { }

  StreamSocket& socket() { return socket_; }
  Address const& peer_address() const { return peer_address_; }

  bool handle_timer()
  {
    std::cout << "Timer ticks" << std::endl;
    return true;
  }

  virtual void handle_close()
  {
    std::cout << "Connection closed, removing channel " << std::endl;
    delete this;
  }

private:
  StreamSocket socket_;
  Address peer_address_;
};

} } // namespace ku::net

