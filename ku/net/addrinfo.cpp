#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <cstdio>
#include "addrinfo.hpp"

namespace ku { namespace net {

AddrInfo::AddrInfo() : error_(0)
{
  addrinfo_ = static_cast<addrinfo*>(malloc(sizeof(addrinfo)));
  ::bzero(addrinfo_, sizeof(addrinfo));
  addrinfo_->ai_family = AF_INET;
  addrinfo_->ai_protocol = TCP;
  addrinfo_->ai_socktype = Stream;
  addrinfo_->ai_flags = Passive;
}

AddrInfo::AddrInfo(Address const& addr, Family fml, Protocol proto, Flag flag)
  : addrinfo_(nullptr)
{
  char buf[8];
  sprintf(buf, ":%u", addr.port());

  addrinfo hints;
  hints.ai_family = fml;
  hints.ai_protocol = proto;
  hints.ai_flags = flag;
  hints.ai_socktype = proto == UDP ? Datagram : Stream;
  error_ = ::getaddrinfo(addr.ip().c_str(), buf, &hints, &addrinfo_);
}

} } // namespace ku::net
