#pragma once
#include "notice.hpp"
#include "socket.hpp"
#include "timer.hpp"

namespace ku { namespace net {

inline Notice::Type notice_type(StreamSocket const& sock)
{
  return Notice::Connection;
}

inline Notice::Type notice_type(AcceptorSocket const& sock)
{
  return Notice::Acceptor;
}

inline Notice::Type notice_type(Timer const&)
{
  return Notice::Timer;
}

// Caller set type depending on context
inline Notice::Type notice_type(Handle const&)
{
  return Notice::None;
}

} } // namespace ku::net

