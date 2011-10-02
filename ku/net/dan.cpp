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
  channel::handle channel = channel::create(socket, adr);
  socket::close(socket);
}

TEST(poller, handle)
{
  addrinfo addr;
  socket::handle socket = socket::create(addr);
  address adr;
  channel::handle channel = channel::create(socket, adr);
  poller::handle poller = poller::create(); 
  poller::add_channel(poller, channel);
  poller::close(poller);
}

