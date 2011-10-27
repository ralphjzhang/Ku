#include "connector.hpp"

namespace ku { namespace net {

bool Connector::connect(Endpoint const& endpoint)
{
  if (socket().connect(endpoint))
    endpoint_ = endpoint;
}

} } // namespace ku::net

