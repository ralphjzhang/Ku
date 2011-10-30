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

namespace ku { namespace net {

class TCPServer
{
public:
  TCPServer() { }

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
  epoll::PollLoop loop_;
  std::thread thread_;
};

} }

