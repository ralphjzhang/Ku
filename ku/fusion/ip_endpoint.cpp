/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <system_error>
#include "util.hpp"
#include "endpoint.hpp"
#include "ip_endpoint.hpp"

namespace {

int make_sockaddr(std::string ip, std::string port, sockaddr_storage& sa_storage)
{
  std::memset(&sa_storage, 0, sizeof(sockaddr_storage));
  if (*ip.begin() == '[' && *ip.rbegin() == ']') {
    sockaddr_in6& sa_in6 = *reinterpret_cast<sockaddr_in6*>(&sa_storage);
    sa_in6.sin6_family = AF_INET6;
    sa_in6.sin6_port = ::htons(std::stoi(port));
    std::string ipv6 = ip.substr(1, ip.size() - 2);
    if (ipv6 == "*")
      sa_in6.sin6_addr = in6addr_any;
    else
      return ::inet_pton(AF_INET6, ipv6.c_str(), &sa_in6.sin6_addr);
  } else {
    sockaddr_in& sa_in = *reinterpret_cast<sockaddr_in*>(&sa_storage);
    sa_in.sin_family = AF_INET;
    sa_in.sin_port = ::htons(std::stoi(port));
    if (ip == "*")
      sa_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
    else
      return ::inet_pton(AF_INET, ip.c_str(), &sa_in.sin_addr);
  }
  return 0;
}

} // unamed namespace

namespace ku { namespace fusion {

IPEndpoint::IPEndpoint(Endpoint const& endpoint)
{
  Protocol p = endpoint.protocol();
  if (p != Protocol::TCP && p != Protocol::PGM && p != Protocol::WS);
    throw std::system_error(util::errc(EINVAL), "IPEndpoint::IPEndpoint");
  // "address" of Endpoint is actually "endpoint" of IP protocol
  std::string const& ep = endpoint.address();
  size_t pos = ep.find_last_of(':');
  if (pos == std::string::npos || pos == ep.size() - 1)
    throw std::system_error(util::errc(EINVAL), "IPEndpoint::IPEndpoint");

  if (make_sockaddr(ep.substr(0, pos), ep.substr(pos + 1), sockaddr_.sa_storage) == -1)
    throw std::system_error(util::errc(), "IPEndpoint::IPEndpoint");
}

std::string IPEndpoint::address() const
{
  // TODO error handling
  if (IPv4 == address_family_) {
    char ip[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &sockaddr_.sa_in.sin_addr, ip, sizeof(ip));
    return ip;
  } else {
    char ip[INET6_ADDRSTRLEN];
    ::inet_ntop(AF_INET6, &sockaddr_.sa_in6.sin6_addr, ip, sizeof(ip));
    return ip;
  }
}

uint16_t IPEndpoint::port() const
{
  if (IPv4 == address_family_)
    return ::ntohs(sockaddr_.sa_in.sin_port);
  else
    return ::ntohs(sockaddr_.sa_in6.sin6_port);
}

} } // namespace ku::fusion

