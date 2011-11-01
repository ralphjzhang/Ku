/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <thread>
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

  void start()
  {
    std::thread(std::ref(loop_)).swap(thread_);
  }

  void stop()
  {
    loop_.quit();
    thread_.join();
  }

private:
  std::thread thread_;
  epoll::PollLoop loop_;
  SocketAcceptor<Connection> acceptor_;
};

} } // namespace ku::net

