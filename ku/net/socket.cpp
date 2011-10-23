#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "address.hpp"
#include "socket.hpp"

namespace ku { namespace net {

Socket::Socket(addrinfo const& addr)
  : Handle(::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol), true)
{
  if (raw_handle()) {
    ku::net::set_non_block(*this);
    ku::net::set_close_exec(*this);
  } else {
    set_error(errno);
  }
}

} } // namespace ku::net


