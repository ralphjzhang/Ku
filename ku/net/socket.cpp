#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include <ku/util/cast.hpp>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"

namespace {

sockaddr const* sockaddr_cast(sockaddr_in const* addr)
{
  return static_cast<sockaddr const*>(implicit_cast<void const*>(addr));
}

sockaddr* sockaddr_cast(sockaddr_in* addr)
{
  return static_cast<sockaddr*>(implicit_cast<void*>(addr));
}

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

/*
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

bool self_connect(int socket_fd)
{
  sockaddr_in local_addr = get_local_addr(socket_fd),
              peer_addr = get_peer_addr(socket_fd);
  return local_addr.sin_port == peer_addr.sin_port
      && local_addr.sin_addr.s_addr == peer_addr.sin_addr.s_addr;
handle create(socket::handle& socket);
}*/

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
  if (::bind(h.raw_handle(), sockaddr_cast(&addr.sockaddr()), sizeof(addr)) == -1) {
    h.set_error(errno);
    return h;
  }
  if (::listen(h.raw_handle(), SOMAXCONN) == -1)
  {
    h.set_error(errno);
    return h;
  }
  return h;
}

channel::handle accept(handle& h, address const& addr)
{
  socklen_t addr_len = sizeof(addr);
  int conn_fd = ::accept4(h.raw_handle(), sockaddr_cast(const_cast<sockaddr_in*>(&addr.sockaddr())),
      &addr_len, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd == -1)
    return channel::handle(conn_fd, errno);
  return channel::handle(conn_fd);
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


