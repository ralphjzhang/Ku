#include <cassert>
#include "handle_ops.hpp"
#include "channel_ops.hpp"

namespace ku { namespace net {

/*
Channel accept(Channel& chan, Address& addr)
{
  assert(chan.type() == Channel::Acceptor);
  Channel conn_chan;
  conn_chan.adopt(ku::net::accept(chan.handle(), addr));
  conn_chan.set_event_type(Channel::In);
  return conn_chan;
}
*/

} } // namespace ku::net
