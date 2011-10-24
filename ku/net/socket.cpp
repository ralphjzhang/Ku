#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "address.hpp"
#include "addrinfo.hpp"
#include "util.hpp"
#include "socket.hpp"

namespace ku { namespace net {

/// StreamSocket ///


/// AcceptorSocket ///

AcceptorSocket::AcceptorSocket(Address const& address)
  : handle_(ku::net::socket(AddrInfo()))
{
  if (ku::net::set_non_block(handle_) && ku::net::set_close_exec(handle_))
    listen(address);
}

} } // namespace ku::net


