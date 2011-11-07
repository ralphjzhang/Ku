/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <utility>
#include "epoll_poller.hpp" 
#include "socket_acceptor.hpp"

namespace ku { namespace net {

template <typename EventHandler>
class TCPServer
{
  typedef ServerConnection<EventHandler> Connection;

public:
  TCPServer(Endpoint const& local_endpoint)
    : acceptor_(local_endpoint, loop_.notices()) { }

  bool operator()() { return loop_(); }

  void stop() { loop_.quit(); }

private:
  epoll::PollLoop loop_;
  SocketAcceptor<Connection> acceptor_;
};

} } // namespace ku::net

