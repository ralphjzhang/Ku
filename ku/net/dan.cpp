#include <netdb.h>
#include <functional>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "handle_ops.hpp"
#include "socket.hpp"
#include "timer.hpp"
#include "channel_ops.hpp"
#include "channel.hpp"
#include "epoll_poller.hpp"
#include "poll_poller.hpp"
#include "loop.hpp"
#include "acceptor.hpp"

using namespace ku;
using namespace ku::net;

template <typename HandleType>
void test_owner_handle(HandleType&& h)
{
  EXPECT_TRUE(!h.error());
  EXPECT_TRUE(h.owner());
  int raw_handle = h.raw_handle();

  HandleType h2(std::move(h));
  EXPECT_TRUE(h2.owner()); EXPECT_FALSE(h.owner());
  EXPECT_EQ(h2.raw_handle(), raw_handle);

  h2.release_handle();
  EXPECT_FALSE(h2.owner());
  EXPECT_EQ(h2.raw_handle(), 0);

  h2.close();
  EXPECT_TRUE(!h2.error());
  EXPECT_EQ(h2.raw_handle(), 0);
}

TEST(Socket, handle)
{
  Socket sock = Socket::create(aif());
  test_owner_handle(std::move(sock));
}

Socket make_listener()
{
  Address addr("127.0.0.1", 8888);
  Socket sock = Socket::create(aif());
  sock.listen(addr);
  return sock;
}

TEST(Socket, listen)
{
  Socket sock = make_listener();
  EXPECT_TRUE(!sock.error());
  EXPECT_TRUE(sock.listening());
}

TEST(Timer, handle)
{
  Timer timer = Timer::create();
  test_owner_handle(std::move(timer));
}

TEST(Timer, interval)
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
  Socket sock = make_listener();
  int sock_fd = sock.raw_handle();
  Channel chan;
  chan.adopt(std::move(sock));
  EXPECT_EQ(chan.type(), Channel::Acceptor);
  EXPECT_TRUE(chan.owner()); EXPECT_FALSE(sock.owner());
  EXPECT_EQ(chan.raw_handle(), sock_fd);
  EXPECT_EQ(sock.raw_handle(), 0);
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
  bool handle_accept(Channel const&, Address const& addr)
  {
    std::cout << "Connection from: " << to_str(addr) << std::endl;
    return true;
  }
  bool handle_read(Channel& chan)
  {
    char buf[10];
    read(chan, buf, 10);
    std::cout << "We have some data to read: " << buf << std::endl;
    strcpy(buf, "World");
    write(chan, buf, 6);
    exit(0);
  }
  bool handle_timer(Channel& chan)
  {
    std::cout << "Timer ticks" << std::endl;
    return true;
  }
  bool handle_close(Channel& chan, ChannelHub& hub)
  {
    // TODO this is fatal for poll::Poller
    std::cout << "Connection closed, removing channel" << std::endl;
    hub.remove_channel(chan.raw_handle());
    exit(0);
  }
};

void epoll_test()
{
  Address addr("127.0.0.1", 8888);
  std::error_code err = server_loop<Epoll>(addr, Handler());
  if (err)
    std::cout << err.message() << std::endl;
}

void poll_test()
{
  Address addr("127.0.0.1", 8888);
  server_loop<Poll>(addr, Handler());
}

int main()
{
  epoll_test();
  //poll_test();
}

