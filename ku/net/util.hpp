#pragma once
#include <system_error>

struct sockaddr;
struct sockaddr_in;

namespace ku { namespace net { namespace util {

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}

struct noncopyable
{
  noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
};


inline sockaddr const* sockaddr_cast(sockaddr_in const* addr)
{
  return static_cast<sockaddr const*>(implicit_cast<void const*>(addr));
}

inline sockaddr* sockaddr_cast(sockaddr_in* addr)
{
  return static_cast<sockaddr*>(implicit_cast<void*>(addr));
}

bool self_connect(int socket_fd);


sockaddr_in get_local_addr(int socket_fd);

sockaddr_in get_peer_addr(int socket_fd);

} } } // namespace ku::net::util

