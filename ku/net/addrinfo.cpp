#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <cstdio>
#include "addrinfo.hpp"

namespace ku { namespace net {

AddrInfo AddrInfo::create()
{
  // TODO is ok to freeaddrinfo a malloc-ed addrinfo?
  addrinfo* ai = static_cast<addrinfo*>(malloc(sizeof(addrinfo)));
  ::bzero(ai, sizeof(addrinfo));
  ai->ai_family = AF_INET;
  ai->ai_protocol = TCP;
  ai->ai_socktype = Stream;
  ai->ai_flags = Passive;
  return AddrInfo(ai);
}

AddrInfo AddrInfo::create(unsigned port, Family family, Protocol protocol, Flag flag)
{
  char buf[8];
  sprintf(buf, ":%u", port);
  return create(buf, nullptr, family, protocol, flag);
}

AddrInfo AddrInfo::create(char const* service, char const* node, Family family, Protocol protocol,
    Flag flag)
{
  addrinfo hints;
  hints.ai_family = family;
  hints.ai_protocol = protocol;
  hints.ai_flags = flag;
  hints.ai_socktype = protocol == UDP ? Datagram : Stream;
  // TODO get errors
  addrinfo* ai_ptr = nullptr;
  int ret = ::getaddrinfo(node, service, &hints, &ai_ptr);
  if (ret == 0)
    return AddrInfo(ai_ptr);
  return AddrInfo();
}

AddrInfo::~AddrInfo()
{
  if (addrinfo_)
    ::freeaddrinfo(addrinfo_);
}

} } // namespace ku::net
