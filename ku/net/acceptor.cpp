#include "acceptor.hpp"
#include "address.hpp"

namespace ku { namespace net {

Acceptor::Acceptor(addrinfo const& aif, Address const& addr)
  : socket_(Socket::create(aif))
{
  socket_.listen(addr);
}

  /*
ChannelList Acceptor::accept()
{
  ChannelList channels;
  while (true) {
    Address addr;
    Socket conn_sock = socket_.accept(addr);
    if (conn_sock.error()) {
      if (conn_sock.error() == std::errc::operation_would_block)
        break;
      else {
        ; // TODO error 
      }
    }
    channels.add_channel(Channel(conn_sock, Channel::In));
    conn_sock.clear();
  }
  // return std::move(channels);
}
  */

} } // namespace ku::net

