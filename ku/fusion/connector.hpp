#pragma once
#include "util.hpp"
#include "socket_endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion {

class Connector : private util::noncopyable
{
public:
  Connector() = default;

  void connect(SocketEndpoint const& endpoint);
  ConnectorSocket& socket() { return socket_; }

private:
  SocketEndpoint endpoint_;
  ConnectorSocket socket_;
};

} } // namespace ku::fusion

