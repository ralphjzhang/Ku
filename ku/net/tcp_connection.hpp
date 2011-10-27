#pragma once
#include <iostream>
#include <cstring>
#include "endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class TCPConnection
{
public:
  TCPConnection(StreamSocket&& socket, Endpoint const& peer_endpoint) 
    : socket_(std::move(socket)), peer_endpoint_(peer_endpoint)
  { }
  virtual ~TCPConnection() { }

  StreamSocket& socket() { return socket_; }
  Endpoint const& peer_endpoint() const { return peer_endpoint_; }

  bool handle_timer()
  {
    std::cout << "Timer ticks" << std::endl;
    return true;
  }

  virtual void handle_close()
  {
    std::cout << "Connection closed, removing notice " << std::endl;
    delete this;
  }

private:
  StreamSocket socket_;
  Endpoint peer_endpoint_;
};

} } // namespace ku::net

