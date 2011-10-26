#include "connector.hpp"

namespace ku { namespace net {

bool Connector::connect(Address const& addr)
{
  if (socket().connect(addr))
    address_ = addr;
}

} } // namespace ku::net

