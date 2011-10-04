#include <netdb.h>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "poller.hpp"

using namespace ku;
using namespace ku::net;

TEST(Socket, handle)
{
  addrinfo addr;
  Socket sock = Socket::create(addr);
  close(sock);
}

TEST(Channel, handle)
{
  addrinfo addr;
  Socket sock = Socket::create(addr);
  close(sock);
  Channel chan(sock);
  close(sock);
}

TEST(poller, handle)
{
  addrinfo addr;
  Socket sock = Socket::create(addr);
  close(sock);
  Channel chan(sock);
  poller::handle poller = poller::create(); 
  poller::add_channel(poller, chan);
  poller::close(poller);
  close(sock);
}

int main()
{
  addrinfo addr;
  addr.ai_family = AF_INET;
  addr.ai_socktype = SOCK_STREAM;
  addr.ai_protocol = IPPROTO_TCP;
  addr.ai_flags = AI_PASSIVE;
  Socket sock = Socket::create(addr);
  Address adr("127.0.0.1", 8888);
  if (sock.bind_listen(adr).error())
    std::cout << sock.error().message() << std::endl;
  Channel chan(sock, to_int(poller::EventsType::Read));

  poller::handle poller = poller::create(); 
  poller::add_channel(poller, chan);
  poller::events evts(16);

  while (1) {
    poller::poll(poller, evts, std::chrono::milliseconds(100000));
    poller::ChannelList chans;
    poller::dispatch(evts, chans);
    auto in_sock = sock.accept(adr);
    if (in_sock.error())
      std::cout << in_sock.error().message() << std::endl;
    else
      std::cout << "Accepted incoming connection." << std::endl;
    poller::add_channel(poller, Channel(in_sock, to_int(poller::EventsType::Read)));
    if (poller.error())
      std::cout << poller.error().message() << std::endl;
    break;
  }

  poller::close(poller);
  close(sock);
}


