#pragma once
#include <ku/util/noncopyable.hpp>
#include "socket.hpp"
#include "channel.hpp"

namespace ku { namespace net {

class Address;

class Acceptor : ku::util::noncopyable
{
public:
  Acceptor(addrinfo const& aif, Address const& addr);

  //ChannelList accept();
  Socket const& socket() { return socket_; }

  std::error_code error() { return socket_.error(); }

private:
  Socket socket_;
};

} } // namespace ku::net

