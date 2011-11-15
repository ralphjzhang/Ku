#include "connector.hpp"

namespace ku { namespace fusion {

void Connector::connect(SocketEndpoint const& endpoint)
{
  socket().connect(endpoint, false);
  endpoint_ = endpoint;
}

} } // namespace ku::fusion

