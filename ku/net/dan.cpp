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

addrinfo addr()
{
  addrinfo addr;
  addr.ai_family = AF_INET;
  addr.ai_socktype = SOCK_STREAM;
  addr.ai_protocol = IPPROTO_TCP;
  addr.ai_flags = AI_PASSIVE;
  return addr;
}

TEST(Socket, handle)
{
  Socket sock = Socket::create(addr());
  sock.close();
}

TEST(Channel, handle)
{
  Socket sock = Socket::create(addr());
  sock.close();
  Channel chan;
  chan.adopt(sock);
}

TEST(epoll, handle)
{
  Channel chan;
  chan.adopt(Socket::create(addr()));
  auto poller = epoll::Poller::create(); 
  poller.close();
}

struct print_event 
{
  void operator () (Channel& ch) { std::cout << ku::net::to_str(ch.events()) << std::endl; }
};

template <typename Events>
void handler(Channel& ch, Events& evts)
{
  if (ch.has_event(Channel::Read)) {
    if (ch.listening()) {
      Address adr;
      Channel conn_ch(ch.accept(adr));
      if (conn_ch.error())
        std::cout << conn_ch.error().message() << std::endl;
      else {
        evts.adopt_channel(std::move(conn_ch));
        std::cout << "Connection accepted." << std::endl;
      }
    }
    else {
      char buf[10];
      ::read(ch.raw_handle(), buf, 10);
      std::cout << "We have some data to read: " << buf << std::endl;
      strcpy(buf, "World");
      ::write(ch.raw_handle(), buf, 6);
      exit(0);
    }
  }
};

void epoll_test()
{
  Channel ch(Channel::Listen);
  ch.adopt(Socket::create(addr()));
  ch.listen(Address("127.0.0.1", 8888));
  if (ch.error())
    std::cout << ch.error().message() << std::endl;

  auto poller = epoll::Poller::create(); 
  poller.events().adopt_channel(std::move(ch));

  using namespace std::placeholders;
  loop(poller, std::bind(&handler<epoll::Events>, _1, std::ref(poller.events())));
}

void poll_test()
{
  Channel ch(Channel::Listen);
  ch.adopt(Socket::create(addr()));
  ch.listen(Address("127.0.0.1", 8888));
  if (ch.error())
    std::cout << ch.error().message() << std::endl;

  auto poller = poll::Poller::create(); 
  poller.events().adopt_channel(std::move(ch));

  using namespace std::placeholders;
  loop(poller, std::bind(&handler<poll::Events>, _1, std::ref(poller.events())));
}
 
int main()
{
  //epoll_test();
  poll_test();
}


