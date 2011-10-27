#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include "util.hpp"

namespace ku { namespace net { namespace util {

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& endpoint)
{
  endpoint.sin_family = AF_INET;
  endpoint.sin_port = ::htons(port);
  if (::inet_pton(AF_INET, ip, &endpoint.sin_addr) <= 0)
    return std::make_error_code(static_cast<std::errc>(errno));
  return std::error_code();
}

void make_sockaddr(uint16_t port, sockaddr_in& endpoint)
{
  ::bzero(&endpoint, sizeof endpoint);
  endpoint.sin_family = AF_INET;
  endpoint.sin_addr.s_addr = ::htonl(INADDR_ANY);
  endpoint.sin_port = ::htons(port);
}

std::string ip_str(sockaddr_in const& endpoint)
{
  char ip[INET_ADDRSTRLEN];
  return inet_ntop(AF_INET, &endpoint.sin_addr, ip, INET_ADDRSTRLEN);
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
              peer_endpoint = get_peer_addr(socket_fd);
  return local_addr.sin_port == peer_endpoint.sin_port
      && local_addr.sin_addr.s_addr == peer_endpoint.sin_addr.s_addr;
}

sockaddr_in get_local_addr(int socket_fd)
{
  sockaddr_in endpoint;
  bzero(&endpoint, sizeof(endpoint));
  socklen_t addr_len = sizeof(endpoint);
  getsockname(socket_fd, sockaddr_cast(&endpoint), &addr_len);
  return endpoint;
}

sockaddr_in get_peer_addr(int socket_fd)
{
  sockaddr_in endpoint;
  bzero(&endpoint, sizeof(endpoint));
  socklen_t addr_len = sizeof(endpoint);
  getpeername(socket_fd, sockaddr_cast(&endpoint), &addr_len);
  return endpoint;
}


} } } // namespace ku::net::util
