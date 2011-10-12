#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <system_error>

#include "address.hpp"
#include "util.hpp"

namespace ku { namespace net { namespace sys {

inline std::error_code errno_code()
{
  return std::make_error_code(static_cast<std::errc>(errno));
}

inline std::error_code no_error()
{
  return std::error_code();
}

inline std::error_code bind(int fd, Address const& addr)
{
  return ::bind(fd, util::sockaddr_cast(&addr.sockaddr()), sizeof(sockaddr)) == -1 ?
         errno_code() : no_error();         
}

inline std::error_code listen(int fd)
{
  return ::listen(fd, SOMAXCONN) == -1 ? errno_code() : no_error();
}

inline std::pair<int, std::error_code> accept(int fd, Address& addr)
{
  socklen_t addr_len = sizeof(sockaddr_in);
  int conn_fd = ::accept4(fd, util::sockaddr_cast(&addr.sockaddr()), &addr_len,
      SOCK_NONBLOCK | SOCK_CLOEXEC);
  return conn_fd == -1 ? std::make_pair(conn_fd, errno_code()) 
                       : std::make_pair(conn_fd, no_error());
}

inline std::error_code set_non_block(int fd)
{
  int flags = ::fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    return errno_code();
  return ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1 ? errno_code() : no_error();
}

inline std::error_code set_close_exec(int fd)
{
  int flags = ::fcntl(fd, F_GETFD, 0);
  if (flags == -1)
    return errno_code();
  return ::fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1 ? errno_code() : no_error();
}

} } } // namespace ku::net::sys

