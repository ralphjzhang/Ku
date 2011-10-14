#include "sys.hpp"

namespace ku { namespace net { namespace sys {

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr)
{
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(port);
  if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
    return errno_code();
  return no_error();
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

} } } // namespace ku::net::sys

