#pragma once
#include <sys/types.h>
#include <netdb.h>
#include "util.hpp"

namespace ku { namespace net {

class AddrInfo : private util::noncopyable
{
  enum SockType { Stream = SOCK_STREAM, Datagram = SOCK_DGRAM, Raw = SOCK_RAW };
  AddrInfo(addrinfo* ai) : addrinfo_(ai) { }

public:
  enum Family { IPv4 = AF_INET, IPv6 = AF_INET6, Both = AF_UNSPEC };
  enum Protocol { Auto = 0, TCP = IPPROTO_TCP, UDP = IPPROTO_UDP };
  enum Flag { Passive = AI_PASSIVE, CanonName = AI_CANONNAME };

  static AddrInfo create();
  static AddrInfo create(unsigned port, Family family = Both, Protocol protocol = TCP, Flag flag = Passive);

  static AddrInfo create(char const* service, char const* node = nullptr, Family family = Both, Protocol protocol = TCP, Flag flag = Passive);
  
  AddrInfo() : addrinfo_(nullptr) { }
  AddrInfo(AddrInfo&& ai) : addrinfo_(ai.addrinfo_) { ai.addrinfo_ = nullptr; }
  ~AddrInfo();

  operator addrinfo const& () { return *addrinfo_; }

private:
  addrinfo* addrinfo_;
};

} } // namespace ku::net

