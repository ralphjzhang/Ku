#include <sys/types.h>
#include <netdb.h>
#include <ku/dan/dan.hpp>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "poller.hpp"

using namespace ku::net;

TEST(socket, handle)
{
  addrinfo addr;
  socket::handle socket = socket::create(addr);
  socket::close(socket);
}

TEST(channel, handle)
{
  addrinfo addr;
  socket::handle socket = socket::create(addr);
  address adr;
  channel chan(socket);
  socket::close(socket);
}

TEST(poller, handle)
{
  addrinfo addr;
  socket::handle socket = socket::create(addr);
  address adr;
  channel chan(socket);
  poller::handle poller = poller::create(); 
  poller::add_channel(poller, chan);
  poller::close(poller);
  socket::close(socket);
}

int main()
{
  addrinfo addr;
  addr.ai_family = AF_INET;
  addr.ai_socktype = SOCK_STREAM;
  addr.ai_protocol = IPPROTO_TCP;
  addr.ai_flags = AI_PASSIVE;
  address adr("127.0.0.1", 8888);
  socket::handle socket = socket::create(addr);
  socket::bind_listen(socket, adr);
  if (socket.error())
    std::cout << socket.error().message() << std::endl;
  channel chan(socket.raw_handle(), to_int(poller::events_type::Read));

  poller::handle poller = poller::create(); 
  poller::add_channel(poller, chan);
  if (poller.error())
    std::cout << poller.error().message() << std::endl;
  poller::events evts(16);
  poller::poll(poller, evts, std::chrono::milliseconds(100000));

  poller::close(poller);
  socket::close(socket);
}


