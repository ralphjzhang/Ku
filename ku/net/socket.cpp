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
  flags = ::fcntl(socket_fd, F_SETFL, flags);
  if (flags == -1)
    return errno;
  return 0;
}

} // unamed namespace

namespace ku { namespace net { namespace socket {

handle create(addrinfo const& addr)
{
  int socket_fd = ::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
  if (socket_fd == -1)
    return handle(socket_fd, errno);
  int error_no = set_non_block(socket_fd);
  return handle(socket_fd, error_no);
}

handle& bind_listen(handle& h, address const& addr)
{
  if (::bind(h.raw_handle(), util::sockaddr_cast(&addr.sockaddr()), sizeof(sockaddr)) == -1) {
    h.set_error(errno);
    return h;
  }
  if (::listen(h.raw_handle(), SOMAXCONN) == -1) {
    h.set_error(errno);
    return h;
  }
  return h;
}

handle accept(handle const& h, address& addr)
{
  socklen_t addr_len = sizeof(sockaddr_in);
  int socket_fd = ::accept4(h.raw_handle(), util::sockaddr_cast(&addr.sockaddr()), &addr_len,
      SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (socket_fd == -1)
    return handle(socket_fd, errno);
  return handle(socket_fd);
}

ssize_t read(handle const& h, void* buf, size_t count)
{
  return ::read(h.raw_handle(), buf, count);
}

ssize_t write(handle const& h, void* buf, size_t count)
{
  return ::write(h.raw_handle(), buf, count);
}

handle& close(handle& h)
{
  if (::close(h.raw_handle()) == -1)
    h.set_error(errno);
  return h;
}

} } } // namespace ku::net::socket


