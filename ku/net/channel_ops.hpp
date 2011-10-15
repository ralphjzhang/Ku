#pragma once
#include "channel.hpp"
#include "socket.hpp"
#include "timer.hpp"

namespace ku { namespace net {

inline Channel::Type channel_type(Socket const& sock)
{
  return sock.listening() ? Channel::Acceptor : Channel::Connection;
}

inline Channel::Type channel_type(Timer const& timer)
{
  return Channel::Timer;
}

Channel accept(Channel& chan, Address& addr);

} } // namespace ku::net

