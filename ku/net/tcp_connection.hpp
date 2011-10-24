#pragma once
#include <iostream>
#include <cstring>
#include "socket.hpp"

namespace ku { namespace net {

class TCPConnection
{
public:
  TCPConnection(StreamSocket&& socket) : socket_(std::move(socket)) { }
  ~TCPConnection() { std::cout << "~TCPConnection" << std::endl; }

  StreamSocket& socket() { return socket_; }

  bool handle_read()
  {
    char buf[10];
    if (socket_.read(buf, 10) > 0)
      std::cout << "We have some data to read: " << buf << std::endl;
    //write(chan, buf, 6);
    return true;
  }

  bool handle_timer()
  {
    std::cout << "Timer ticks" << std::endl;
    return true;
  }

  void handle_close()
  {
    std::cout << "Connection closed, removing channel " << std::endl;
    delete this;
  }

private:
  StreamSocket socket_;
};

} };
