#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <cstdio>
#include "resolver.hpp"

namespace ku { namespace fusion {

Resolver::Resolver(Flag flag) : error_(0)
{
  addrinfo_ = static_cast<addrinfo*>(malloc(sizeof(addrinfo)));
  ::bzero(addrinfo_, sizeof(addrinfo));
  addrinfo_->ai_family = AF_INET;
  addrinfo_->ai_protocol = TCP;
  addrinfo_->ai_socktype = Stream;
  addrinfo_->ai_flags = Passive;
}

Resolver::Resolver(Endpoint const& endpoint, Family fml, Protocol proto, Flag flag)
  : addrinfo_(nullptr)
{
  char buf[8];
  sprintf(buf, ":%u", endpoint.port());

  addrinfo hints;
  hints.ai_family = fml;
  hints.ai_protocol = proto;
  hints.ai_flags = flag;
  hints.ai_socktype = proto == UDP ? Datagram : Stream;
  error_ = ::getaddrinfo(endpoint.ip().c_str(), buf, &hints, &addrinfo_);
}

} } // namespace ku::fusion
