#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include "util.hpp"
#include "address.hpp"
#include "socket.hpp"

namespace {

int set_non_block(int socket_fd)
{
  int flags = ::fcntl(socket_fd, F_GETFL, 0);
  if (flags == -1)
    return errno;
  flags |= O_NONBLOCK;
  int ret = ::fcntl(socket_fd, F_SETFL, flags);
  if (ret == -1)
    return errno;

  flags = ::fcntl(socket_fd, F_GETFL, 0);
  flags |= FD_CLOEXEC;
  ret = ::fcntl(socket_fd, F_SETFD, flags);
  if (ret == -1)
    return errno;

  return 0;
}

} // unamed namespace

namespace ku { namespace net {

Socket Socket::create(addrinfo const& addr)
{
  int socket_fd = ::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
  if (socket_fd == -1)
    return Socket(socket_fd, errno);
  int error_no = set_non_block(socket_fd);
  return Socket(socket_fd, error_no);
}

Socket& Socket::bind_listen(Address const& addr)
{
  if (::bind(raw_handle(), util::sockaddr_cast(&addr.sockaddr()), sizeof(sockaddr)) == -1) {
    set_error(errno);
    return *this;
  }
  if (::listen(raw_handle(), SOMAXCONN) == -1) {
    set_error(errno);
    return *this;
  }
  return *this;
}

Socket Socket::accept(Address& addr) const
{
  socklen_t addr_len = sizeof(sockaddr_in);
  int socket_fd = ::accept4(raw_handle(), util::sockaddr_cast(&addr.sockaddr()), &addr_len,
      SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (socket_fd == -1)
    return Socket(socket_fd, errno);
  return Socket(socket_fd);
}

ssize_t read(Socket const& h, void* buf, size_t count)
{
  return ::read(h.raw_handle(), buf, count);
}

ssize_t write(Socket const& h, void* buf, size_t count)
{
  return ::write(h.raw_handle(), buf, count);
}

Socket& close(Socket& h)
{
  if (::close(h.raw_handle()) == -1)
    h.set_error(errno);
  return h;
}

} } // namespace ku::net


