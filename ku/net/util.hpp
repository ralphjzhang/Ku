#pragma once
#include <system_error>
#include <ku/util/cast.hpp>

struct sockaddr;
struct sockaddr_in;

namespace ku { namespace net { namespace util {

inline sockaddr const* sockaddr_cast(sockaddr_in const* addr)
{
  return static_cast<sockaddr const*>(implicit_cast<void const*>(addr));
}

inline sockaddr* sockaddr_cast(sockaddr_in* addr)
{
  return static_cast<sockaddr*>(implicit_cast<void*>(addr));
}

bool self_connect(int socket_fd);

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr);

sockaddr_in get_local_addr(int socket_fd);

sockaddr_in get_peer_addr(int socket_fd);

} } } // namespace ku::net::util

