#pragma once
#include <thread>
#include "epoll_poller.hpp" 

namespace ku { namespace net {

template <typename Dispatcher>
class TCPServer
{
public:
  TCPServer(Dispatcher& dispatcher) : dispatcher_(dispatcher) { }

  void start()
  {
    std::thread([this]() { loop_(dispatcher_); }).swap(thread_);
  }

  void stop()
  {
    loop_.quit();
    thread_.join();
  }

private:
  epoll::PollLoop<Dispatcher> loop_;
  Dispatcher& dispatcher_;
  std::thread thread_;
};

} }

