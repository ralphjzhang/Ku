#include <cassert>
#include "handle_ops.hpp"
#include "channel_ops.hpp"

namespace ku { namespace net {

Channel accept(Channel& chan, Address& addr)
{
  assert(chan.type() == Channel::Acceptor);
  Channel conn_chan(accept(chan.handle_, addr));
  conn_chan.type_ = Channel::Connection;
  conn_chan.set_event_type(Channel::In);
  return conn_chan;
}

} } // namespace ku::net
