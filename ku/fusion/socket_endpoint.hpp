#pragma once
#include <sys/un.h>
#include <netdb.h>
#include <cstring>

namespace ku { namespace fusion {

class Endpoint;

class SocketEndpoint
{
  union SockAddr
  {
    sockaddr sa;
    sockaddr_in sa_in;
    sockaddr_in6 sa_in6;
    sockaddr_un sa_un;
    sockaddr_storage sa_storage;
  };

public:
  enum AddressFamily { Invalid, Unix, IPv4, IPv6 };

  SocketEndpoint() : address_family_(Invalid) { }
  SocketEndpoint(SocketEndpoint const&) = default;
  ~SocketEndpoint() = default;

  SocketEndpoint(Endpoint const& endpoint);
  SocketEndpoint(sockaddr_un const& sa_un) : address_family_(Unix) { sockaddr_.sa_un = sa_un; }
  SocketEndpoint(sockaddr_in const& sa_in) : address_family_(IPv4) { sockaddr_.sa_in = sa_in; }
  SocketEndpoint(sockaddr_in6 const& sa_in6) : address_family_(IPv6) { sockaddr_.sa_in6 = sa_in6; }

  sockaddr const& sockaddress() const { return sockaddr_.sa; }
  AddressFamily address_family() const { return address_family_; }
  size_t sockaddr_size() const
  {
    return address_family_ == 
        Unix ? std::strlen(sockaddr_.sa_un.sun_path) + sizeof(sockaddr_.sa_un.sun_family) 
             : IPv4 ? sizeof(sockaddr_in) : IPv6 ? sizeof(sockaddr_in6) : 0;
  }

  std::string address() const;
  uint16_t port() const; // For Unix domain sockets, port is 0

private:
  AddressFamily address_family_;
  SockAddr sockaddr_;
};

inline std::string to_str(SocketEndpoint const& endpoint)
{
  char buf[10];
  switch (endpoint.address_family())
  {
  case SocketEndpoint::Invalid:
    return "";
  case SocketEndpoint::Unix:
    return endpoint.address();
  default:
    sprintf(buf, "%u", endpoint.port());
    return endpoint.address().append(":").append(buf);
  }
}

} } // namespace ku::fusion

