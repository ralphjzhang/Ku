#pragma once
#include "util.hpp"
#include "socket.hpp"

namespace ku { namespace fusion {

class Connector : private util::noncopyable
{
public:
  Connector() = default;

  void connect(Endpoint const& endpoint);
  ConnectorSocket& socket() { return socket_; }

private:
  Endpoint endpoint_;
  ConnectorSocket socket_;
};

} } // namespace ku::fusion

