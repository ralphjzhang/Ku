#include "acceptor.hpp"
#include "address.hpp"

namespace ku { namespace net {

Acceptor::Acceptor(addrinfo const& aif, Address const& addr)
  : socket_(Socket::create(aif))
  , channel_(socket_, Channel::In)
{
  if (socket_.bind_listen(addr).error())
    ; // handle error std::cout << sock.error().message() << std::endl;
}

} } // namespace ku::net

