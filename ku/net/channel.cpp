#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"

namespace ku { namespace net { namespace channel {

handle create(socket::handle& socket, address const& addr)
{
  return socket::accept(socket, addr);
}

} } } // namespace ku::net::channel

