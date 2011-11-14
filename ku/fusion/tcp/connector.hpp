#pragma once
#include "../util.hpp"
#include "../ip_endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion { namespace tcp {

class Connector : private util::noncopyable
{
public:
  Connector() = default;

  void connect(IPEndpoint const& endpoint);
  ConnectorSocket& socket() { return socket_; }

private:
  IPEndpoint endpoint_;
  ConnectorSocket socket_;
};

} } } // namespace ku::fusion::tcp

