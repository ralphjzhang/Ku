#include "connector.hpp"

namespace ku { namespace fusion { namespace tcp {

void Connector::connect(IPEndpoint const& endpoint)
{
  socket().connect(endpoint);
  endpoint_ = endpoint;
}

} } } // namespace ku::fusion::tcp

