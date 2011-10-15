#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "sys.hpp"
#include "address.hpp"
#include "socket.hpp"

namespace ku { namespace net {

Socket Socket::create(addrinfo const& addr)
{
  int socket_fd = ::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
  if (socket_fd == -1)
    return Socket(socket_fd, errno);
  std::error_code ec = sys::set_non_block(socket_fd);
  if (ec)
    return Socket(socket_fd, ec);
  return Socket(socket_fd, sys::set_close_exec(socket_fd));
}

Socket& Socket::listen(Address const& addr)
{
  set_error(sys::bind(raw_handle(), addr));
  if (!error()) {
    set_error(sys::listen(raw_handle()));
    if (!error())
      listening_ = true;
  }
  return *this;
}

Socket Socket::accept(Address& addr) const
{
  auto ret = sys::accept(raw_handle(), addr);
  return Socket(ret.first, ret.second);
}

} } // namespace ku::net


