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
  : handle_(ku::net::socket_non_block(AddrInfo(AddrInfo::Passive)))
{
  if (handle_)
    listen(address);
}

/// ConnectorSocket ///

ConnectorSocket::ConnectorSocket()
  : StreamSocket(ku::net::socket_block(AddrInfo())) // TODO let user choose
{
}

} } // namespace ku::net


