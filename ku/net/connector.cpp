#include "connector.hpp"

namespace ku { namespace net {

void Connector::connect(Endpoint const& endpoint)
{
  socket().connect(endpoint);
  endpoint_ = endpoint;
}

} } // namespace ku::net

