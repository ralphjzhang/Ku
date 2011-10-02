#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "util.hpp"

namespace {

std::error_code errno_code()
{
  return std::make_error_code(static_cast<std::errc>(errno));
}

std::error_code no_error()
{
  return std::error_code();
}

}

namespace ku { namespace net { namespace util {

bool self_connect(int socket_fd)
{
  sockaddr_in local_addr = get_local_addr(socket_fd),
              peer_addr = get_peer_addr(socket_fd);
  return local_addr.sin_port == peer_addr.sin_port
      && local_addr.sin_addr.s_addr == peer_addr.sin_addr.s_addr;
}

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr)
{
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(port);
  if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
    return errno_code();
  return no_error();
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
