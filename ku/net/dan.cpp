#include <netdb.h>
#include <functional>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "buffer.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "epoll_poller.hpp"
#include "poll_poller.hpp"
#include "loop.hpp"
#include "acceptor.hpp"

using namespace ku;
using namespace ku::net;

TEST(Socket, handle)
{
  Socket sock = Socket::create(aif());
  sock.close();
}

TEST(Channel, handle)
{
  Socket sock = Socket::create(aif());
  sock.close();
  Channel chan;
  chan.adopt(sock);
}

TEST(epoll, handle)
{
  Channel chan;
  chan.adopt(Socket::create(aif()));
  auto poller = epoll::Poller::create(); 
  poller.close();
}

struct Handler
{
  bool handle_connect(Channel&) { return true; }
  bool handle_read(Channel const& chan)
  {
    char buf[10];
    ::read(chan.raw_handle(), buf, 10);
    std::cout << "We have some data to read: " << buf << std::endl;
    strcpy(buf, "World");
    ::write(chan.raw_handle(), buf, 6);
    exit(0);
  }
};

void epoll_test()
{
  Address addr("127.0.0.1", 8888);
  server_loop<poll::Poller>(addr, Handler());
}

void poll_test()
{

  Address addr("127.0.0.1", 8888);
  server_loop<poll::Poller>(addr, Handler());
}
 
int main()
{
  epoll_test();
  //poll_test();
}


