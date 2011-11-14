#pragma once
#include <netdb.h>

namespace ku { namespace fusion {

class Endpoint;

class IPEndpoint
{
  union SockAddr
  {
    sockaddr sa;
    sockaddr_in sa_in;
    sockaddr_in6 sa_in6;
    sockaddr_storage sa_storage;
  };

public:
  enum AddressFamily { Invalid, IPv4, IPv6 };

  IPEndpoint() : address_family_(Invalid) { }
  IPEndpoint(IPEndpoint const&) = default;
  ~IPEndpoint() = default;

  IPEndpoint(Endpoint const& endpoint);
  IPEndpoint(sockaddr_in const& sa_in) : address_family_(IPv4) { sockaddr_.sa_in = sa_in; }
  IPEndpoint(sockaddr_in6 const& sa_in6) : address_family_(IPv6) { sockaddr_.sa_in6 = sa_in6; }

  sockaddr const& sockaddress() const { return sockaddr_.sa; }
  AddressFamily address_family() const { return address_family_; }
  size_t sockaddr_size() const { return address_family_ == IPv4 ? sizeof(sockaddr_in) : sizeof(sockaddr_in6); }

  std::string address() const;
  uint16_t port() const;

private:
  AddressFamily address_family_;
  SockAddr sockaddr_;
};

inline std::string to_str(IPEndpoint const& endpoint)
{
  char buf[10];
  sprintf(buf, "%u", endpoint.port());
  return endpoint.address().append(":").append(buf);
}

} } // namespace ku::fusion

