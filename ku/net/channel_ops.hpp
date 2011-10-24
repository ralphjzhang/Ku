#pragma once
#include "channel.hpp"
#include "socket.hpp"
#include "timer.hpp"

namespace ku { namespace net {

inline Channel::Type channel_type(StreamSocket const& sock)
{
  return Channel::Connection;
}

inline Channel::Type channel_type(AcceptorSocket const& sock)
{
  return Channel::Acceptor;
}

inline Channel::Type channel_type(Timer const&)
{
  return Channel::Timer;
}

// Caller set type depending on context
inline Channel::Type channel_type(Handle const&)
{
  return Channel::None;
}

} } // namespace ku::net

