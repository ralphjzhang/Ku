#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "sys.hpp"
#include "address.hpp"
#include "socket.hpp"

namespace ku { namespace net {

Socket::Socket(Socket&& s)
  : raw_handle_(s.raw_handle_), error_(s.error_), listening_(s.listening_)
{
  s.clear();
}

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

int Socket::release_handle()
{
  listening_ = false;
  int handle = raw_handle_;
  raw_handle_ = 0;
  return handle;
}

Socket& Socket::listen(Address const& addr)
{
  error_ = (sys::bind(raw_handle(), addr));
  if (!error_) {
    error_ = (sys::listen(raw_handle()));
    if (!error_)
      listening_ = true;
  }
  return *this;
}

Socket Socket::accept(Address& addr) const
{
  auto ret = sys::accept(raw_handle(), addr);
  return Socket(ret.first, ret.second);
}

void Socket::close()
{
  if (raw_handle_) {
    if (::close(raw_handle_) == -1)
      set_error(errno);
    else
      clear();
  }
}

} } // namespace ku::net


