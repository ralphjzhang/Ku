#pragma once
#include <sys/types.h>
#include <netdb.h>
#include "util.hpp"
#include "address.hpp"

namespace ku { namespace net {

class AddrInfo : private util::noncopyable
{
  enum SockType { Stream = SOCK_STREAM, Datagram = SOCK_DGRAM, Raw = SOCK_RAW };
  AddrInfo(addrinfo* ai) : addrinfo_(ai) { }

public:
  enum Family { IPv4 = AF_INET, IPv6 = AF_INET6, Both = AF_UNSPEC };
  enum Protocol { Auto = 0, TCP = IPPROTO_TCP, UDP = IPPROTO_UDP };
  enum Flag { Passive = AI_PASSIVE, CanonName = AI_CANONNAME };

  AddrInfo();
  AddrInfo(Address const& addr, Family fml = Both, Protocol proto = TCP, Flag flag = Passive);
  AddrInfo(AddrInfo&& ai) : addrinfo_(ai.addrinfo_), error_(ai.error_)
  { ai.addrinfo_ = nullptr; ai.error_ = 0; }

  ~AddrInfo() { if (addrinfo_) ::freeaddrinfo(addrinfo_); }

  operator addrinfo const& () { return *addrinfo_; }

  int error() const { return error_; }
  std::string error_message() const
  { return error() ? std::string(::gai_strerror(error())) : std::string(); }

private:
  addrinfo* addrinfo_;
  int error_;
};

} } // namespace ku::net

