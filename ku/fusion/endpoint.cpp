#include <cstdio>
#include "endpoint.hpp"
#include "util.hpp"

namespace ku { namespace fusion {

Endpoint::Endpoint(std::string const& ip, uint16_t port)
{
  util::make_sockaddr(ip.c_str(), port, sockaddr_);
}

Endpoint::Endpoint(uint16_t port)
{
  util::make_sockaddr(port, sockaddr_);
}

std::string Endpoint::ip() const
{
  return util::ip_str(sockaddr_);
}

uint16_t Endpoint::port() const
{
  return ::ntohs(sockaddr_.sin_addr.s_addr);
}

std::string to_str(Endpoint const& addr)
{
  std::string ip = addr.ip();
  char port[8];
  sprintf(port, ":%u", addr.port());
  ip.append(port);
  return ip;
}


} } // namespace ku::fusion

