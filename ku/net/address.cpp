#include <cstdio>
#include "address.hpp"
#include "sys.hpp"

namespace ku { namespace net {

Address::Address(std::string const& ip, uint16_t port)
{
  sys::make_sockaddr(ip.c_str(), port, sockaddr_);
}

Address::Address(uint16_t port)
{
  sys::make_sockaddr(port, sockaddr_);
}

std::string Address::ip() const
{
  return sys::ip_str(sockaddr_);
}

uint16_t Address::port() const
{
  return ::ntohs(sockaddr_.sin_addr.s_addr);
}

std::string to_str(Address const& addr)
{
  std::string ip = addr.ip();
  char port[8];
  sprintf(port, ":%u", addr.port());
  ip.append(port);
  return ip;
}


} } // namespace ku::net

