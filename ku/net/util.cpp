#include <netinet/in.h>
#include <arpa/inet.h>
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

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr)
{
  addr.sin_family = AF_INET;
  addr.sin_port = ::htons(port);
  if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
    return errno_code();
  return no_error();
}

} } } // namespace ku::net::util
