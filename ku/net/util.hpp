#pragma once
#include <system_error>

struct sockaddr_in;

namespace ku { namespace net { namespace util {

std::error_code make_sockaddr(char const* ip, uint16_t port, sockaddr_in& addr);

} } } // namespace ku::net::util

