#pragma once
#include "util.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class Connector : private util::noncopyable
{
public:
  Connector() = default;

  bool connect(Endpoint const& endpoint);
  ConnectorSocket& socket() { return socket_; }

  std::error_code error() const { return socket_.error(); }

private:
  Endpoint endpoint_;
  ConnectorSocket socket_;
};

} } // namespace ku::net

