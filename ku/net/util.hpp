#pragma once
#include <system_error>
#include <chrono>

struct sockaddr;
struct sockaddr_in;

namespace ku { namespace net { namespace util {

inline std::error_code errno_code()
{
  return std::make_error_code(static_cast<std::errc>(errno));
}

inline std::error_code no_error()
{
  return std::error_code();
}

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

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr);

void make_sockaddr(uint16_t port, sockaddr_in& addr);

std::string ip_str(sockaddr_in const& addr);


timespec to_timespec(std::chrono::nanoseconds ns);

std::chrono::nanoseconds from_timespec(timespec const& spec);


bool self_connect(int socket_fd);


sockaddr_in get_local_addr(int socket_fd);

sockaddr_in get_peer_addr(int socket_fd);

} } } // namespace ku::net::util

