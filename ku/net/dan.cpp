#include <netdb.h>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "epoll_poller.hpp"
#include "poll_poller.hpp"

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
  Channel chan(sock);
  sock.close();
}

TEST(epoll, handle)
{
  Socket sock = Socket::create(addr());
  Channel chan(sock);
  auto poller = epoll::Poller::create(); 
  poller.close();
  sock.close();
}

void epoll_test()
{
  auto sock = Socket::create(addr());
  Address adr("127.0.0.1", 8888);
  if (sock.bind_listen(adr).error())
    std::cout << sock.error().message() << std::endl;
  Channel chan(sock, to_int(epoll::EventsType::Read));

  auto poller = epoll::Poller::create(); 
  epoll::Events evts(poller, 16);
  evts.add_channel(std::move(chan));

  while (1) {
    poller.poll(evts, std::chrono::milliseconds(100000));
    ChannelList chans;
    evts.dispatch(chans);
    auto in_sock = sock.accept(adr);
    if (in_sock.error())
      std::cout << in_sock.error().message() << std::endl;
    else
      std::cout << "Accepted incoming connection." << std::endl;
    evts.add_channel(Channel(in_sock, to_int(epoll::EventsType::Read)));
    if (poller.error())
      std::cout << poller.error().message() << std::endl;
    break;
  }

  poller.close();
  sock.close();
}

void poll_test()
{
  auto sock = Socket::create(addr());
  Address adr("127.0.0.1", 8888);
  if (sock.bind_listen(adr).error())
    std::cout << sock.error().message() << std::endl;
  Channel chan(sock, to_int(poll::EventsType::Read));

  auto poller = poll::Poller::create(); 
  poll::Events evts(16);
  evts.add_channel(std::move(chan));

  while (1) {
    poller.poll(evts, std::chrono::milliseconds(100000));
    ChannelList chans;
    evts.dispatch(chans);
    auto in_sock = sock.accept(adr);
    if (in_sock.error())
      std::cout << in_sock.error().message() << std::endl;
    else
      std::cout << "Accepted incoming connection." << std::endl;
    evts.add_channel(Channel(in_sock, to_int(poll::EventsType::Read)));
    if (poller.error())
      std::cout << poller.error().message() << std::endl;
    break;
  }
  poller.close();
  sock.close();
}

int main()
{
  epoll_test();
  //poll_test();
}


