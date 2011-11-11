#include "connector.hpp"

namespace ku { namespace fusion {

void Connector::connect(Endpoint const& endpoint)
{
  socket().connect(endpoint);
  endpoint_ = endpoint;
}

} } // namespace ku::fusion

