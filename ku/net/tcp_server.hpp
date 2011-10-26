#pragma once
#include <thread>
#include "epoll_poller.hpp" 

namespace ku { namespace net {

template <typename Dispatcher>
class TCPServer
{
public:
  TCPServer(Dispatcher& dispatcher) : dispatcher_(dispatcher) { }

  template <typename Loop = EpollLoop>
  void start()
  {
    std::thread([this]() { epoll::poll_loop(dispatcher_); }).swap(thread_);
  }

  void stop()
  {
    dispatcher_.quit();
    thread_.join();
  }

private:
  Dispatcher& dispatcher_;
  std::thread thread_;
};

} }

