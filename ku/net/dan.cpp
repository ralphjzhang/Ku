#include <netdb.h>
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

struct print_event 
{
  void operator () (Channel& ch) { std::cout << ku::net::to_str(ch.events()) << std::endl; }
};

void accept_connections(Socket& sock, epoll::Events& evts)
{
  Address adr("127.0.0.1", 8888);
  while (1) {
    auto in_sock = sock.accept(adr);
    if (in_sock.error()) {
      if (in_sock.error() == std::errc::operation_would_block)
        break;
      else {
        std::cout << in_sock.error().message() << std::endl;
        exit(-1);
      }
    }
    evts.add_channel(Channel(in_sock, Channel::In));
    in_sock.release_handle();
  }
}

void epoll_test()
{
  auto sock = Socket::create(addr());
  Address adr("127.0.0.1", 8888);
  Acceptor acceptor(addr(), adr);

  if (sock.bind_listen(adr).error())
    std::cout << sock.error().message() << std::endl;
  Channel chan(sock, Channel::In);

  auto poller = epoll::Poller::create(); 
  epoll::Events evts(poller, 16);
  evts.add_channel(std::move(chan));

  auto handler = [&](Channel& ch) {
    if (ch.raw_handle() == sock.raw_handle()) {
      accept_connections(sock, evts);
      std::cout << "Connection accepted." << std::endl;
    }
    else {
      char buf[10];
      ::read(ch.raw_handle(), buf, 10);
      std::cout << "We have some data to read: " << buf << std::endl;
      strcpy(buf, "World");
      ::write(ch.raw_handle(), buf, 6);
      exit(0);
    }
  };

  loop(poller, evts, handler);
}

void poll_test()
{
  auto sock = Socket::create(addr());
  Address adr("127.0.0.1", 8888);
  if (sock.bind_listen(adr).error())
    std::cout << sock.error().message() << std::endl;

  auto poller = poll::Poller::create(); 
  poll::Events evts(16);
  evts.add_channel(Channel(sock, Channel::In));

  auto handler = [&](Channel& ch) {
    if (ch.raw_handle() == sock.raw_handle()) {
      // new connections
      while (1) {
        auto in_sock = sock.accept(adr);
        if (in_sock.error()) {
          if (in_sock.error() == std::errc::operation_would_block)
            break;
          else {
            std::cout << in_sock.error().message() << std::endl;
            exit(-1);
          }
        }
        evts.add_channel(Channel(in_sock, Channel::In));
        in_sock.release_handle();
      }
    }
    else {
      std::cout << "We have some data to read." << std::endl;
    }
  };

  loop(poller, evts, handler);
}

int main()
{
  epoll_test();
  //poll_test();
}


