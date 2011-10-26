#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "util.hpp"

namespace ku { namespace net { namespace util {

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr)
{
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(port);
  if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
    return std::make_error_code(static_cast<std::errc>(errno));
  return std::error_code();
}

void make_sockaddr(uint16_t port, sockaddr_in& addr)
{
  ::bzero(&addr, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
  addr.sin_port = ::htons(port);
}

std::string ip_str(sockaddr_in const& addr)
{
  char ip[INET_ADDRSTRLEN];
  return inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
  return ip;
}

timespec to_timespec(std::chrono::nanoseconds ns)
{
  using namespace std::chrono;
  seconds sec = duration_cast<seconds>(ns);
  ns = ns - sec;
  return timespec{sec.count(), ns.count()};
}

std::chrono::nanoseconds from_timespec(timespec const& spec)
{
  using namespace std::chrono;
  return seconds(spec.tv_sec) + nanoseconds(spec.tv_nsec);
}

bool self_connect(int socket_fd)
{
  sockaddr_in local_addr = get_local_addr(socket_fd),
              peer_addr = get_peer_addr(socket_fd);
  return local_addr.sin_port == peer_addr.sin_port
      && local_addr.sin_addr.s_addr == peer_addr.sin_addr.s_addr;
}

sockaddr_in get_local_addr(int socket_fd)
{
  sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  socklen_t addr_len = sizeof(addr);
  getsockname(socket_fd, sockaddr_cast(&addr), &addr_len);
  return addr;
}

sockaddr_in get_peer_addr(int socket_fd)
{
  sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  socklen_t addr_len = sizeof(addr);
  getpeername(socket_fd, sockaddr_cast(&addr), &addr_len);
  return addr;
}


} } } // namespace ku::net::util
