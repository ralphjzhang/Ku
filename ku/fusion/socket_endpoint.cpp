/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <system_error>
#include "util.hpp"
#include "endpoint.hpp"
#include "socket_endpoint.hpp"

namespace {

// Make IPv6 socket address
int make_sockaddr(std::string const& ip, std::string const& port, sockaddr_in6& sa_in6)
{
  std::memset(&sa_in6, 0, sizeof(sockaddr_in6));
  sa_in6.sin6_family = AF_INET6;
  sa_in6.sin6_port = ::htons(std::stoi(port));
  std::string ipv6 = ip.substr(1, ip.size() - 2);
  if (ipv6 == "*") {
    sa_in6.sin6_addr = in6addr_any;
    return 1;
  } else {
    return ::inet_pton(AF_INET6, ipv6.c_str(), &sa_in6.sin6_addr);
  }
}

// Make IPv4 socket address
int make_sockaddr(std::string const& ip, std::string const& port, sockaddr_in& sa_in)
{
  std::memset(&sa_in, 0, sizeof(sockaddr_in));
  sa_in.sin_family = AF_INET;
  sa_in.sin_port = ::htons(std::stoi(port));
  if (ip == "*") {
    sa_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
    return 1;
  } else {
    return ::inet_pton(AF_INET, ip.c_str(), &sa_in.sin_addr);
  }
}

// Make Unix domain socket address
int make_sockaddr(std::string const& path, sockaddr_un& sa_un)
{
  sa_un.sun_family = AF_UNIX;
  if (path.empty() || path.size() + 1 >= sizeof(sa_un.sun_path)) {
    errno = EINVAL;
    return -1;
  }
  std::memcpy(sa_un.sun_path, path.c_str(), path.size() + 1);
  return 1;
}

int make_sockaddr(std::string const& ip, std::string const& port, sockaddr_storage& sa_storage)
{
  if (*ip.begin() == '[' && *ip.rbegin() == ']') {
    return make_sockaddr(ip, port, *reinterpret_cast<sockaddr_in6*>(&sa_storage));
  } else {
    return make_sockaddr(ip, port, *reinterpret_cast<sockaddr_in*>(&sa_storage));
  }
  return 0;
}

} // unamed namespace

namespace ku { namespace fusion {

SocketEndpoint::SocketEndpoint(Endpoint const& endpoint)
{
  int ret = -1;
  switch (endpoint.protocol())
  {
  case Protocol::IPC:
    ret = make_sockaddr(endpoint.address(), sockaddr_.sa_un);
    address_family_ = Unix;
    break;
  case Protocol::TCP:
  case Protocol::PGM:
  case Protocol::WS:
    {
      std::string const& ep = endpoint.address();
      size_t pos = ep.find_last_of(':');
      if (pos == std::string::npos || pos == ep.size() - 1) {
        errno = EINVAL;
        break;
      }
      ret = make_sockaddr(ep.substr(0, pos), ep.substr(pos + 1), sockaddr_.sa_storage);
      break;
    }
  default:
    errno = EINVAL;
  }
  if (ret == -1)
    throw std::system_error(util::errc(), "SocketEndpoint::SocketEndpoint");
  switch (sockaddr_.sa_storage.ss_family)
  {
  case AF_UNIX:
    address_family_ = Unix; break;
  case AF_INET:
    address_family_ = IPv4; break;
  case AF_INET6:
    address_family_ = IPv6; break;
  }
}

std::string SocketEndpoint::address() const
{
  char ip[INET6_ADDRSTRLEN];
  // TODO error handling, and maybe we can cache the result
  switch (address_family_)
  {
  case IPv4:
    ::inet_ntop(AF_INET, &sockaddr_.sa_in.sin_addr, ip, INET_ADDRSTRLEN);
    return ip;
  case IPv6:
    ::inet_ntop(AF_INET6, &sockaddr_.sa_in6.sin6_addr, ip, INET6_ADDRSTRLEN);
    return ip;
  case Unix:
    return sockaddr_.sa_un.sun_path;
  default:
    return "";
  }
}

uint16_t SocketEndpoint::port() const
{
  switch (address_family_)
  {
  case IPv4:
    return ::ntohs(sockaddr_.sa_in.sin_port);
  case IPv6:
    return ::ntohs(sockaddr_.sa_in6.sin6_port);
  case Unix:
  default:
    return 0;
  }
}

} } // namespace ku::fusion

