#include <netdb.h>
#include <functional>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "buffer.hpp"
#include "socket.hpp"
#include "timer.hpp"
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
  EXPECT_TRUE(!sock.error());
  sock.close();
  EXPECT_TRUE(!sock.error());
  EXPECT_EQ(sock.raw_handle(), 0);
}

TEST(Timer, handle)
{
  using namespace std::chrono;

  Timer tmr = Timer::create();
  EXPECT_NE(tmr.raw_handle(), -1);
  EXPECT_TRUE(!tmr.error());
  tmr.set_interval(seconds(1));
  EXPECT_TRUE(!tmr.error());
  EXPECT_TRUE(tmr.interval<milliseconds>() == milliseconds(1000));
  EXPECT_TRUE(tmr.interval<microseconds>() == microseconds(1000000));
  tmr.close();
  EXPECT_TRUE(!tmr.error());
  EXPECT_EQ(tmr.raw_handle(), 0);
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
  bool handle_connect(Channel const&, Address const& addr)
  {
    std::cout << "Connection from: " << to_str(addr) << std::endl;
    return true;
  }
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
  server_loop<Epoll>(addr, Handler());
}

void poll_test()
{
  Address addr("127.0.0.1", 8888);
  server_loop<Poll>(addr, Handler());
}
 
/*
int main()
{
  epoll_test();
  //poll_test();
}
*/

