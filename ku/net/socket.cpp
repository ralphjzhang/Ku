#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "endpoint.hpp"
#include "resolver.hpp"
#include "util.hpp"
#include "socket.hpp"

namespace ku { namespace net {

/// StreamSocket ///


/// AcceptorSocket ///

AcceptorSocket::AcceptorSocket(Endpoint const& endpoint)
  : handle_(ku::net::socket_non_block(Resolver(Resolver::Passive)))
{
  if (handle_)
    listen(endpoint);
}

/// ConnectorSocket ///

ConnectorSocket::ConnectorSocket()
  : StreamSocket(ku::net::socket_block(Resolver())) // TODO let user choose
{
}

} } // namespace ku::net


