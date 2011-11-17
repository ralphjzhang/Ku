/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "socket_acceptor.hpp"

namespace ku { namespace fusion {

// TODO by customizing this, we can suppose multiple models:
//      reactor (this one is)
//      thread per connection
//      many threads / many connections
size_t accept_connections(AcceptorSocket& socket, NoticeBoard& notices,
    std::function<Notice::EventHandler(Socket&&, SocketEndpoint const&)> handler_creator)
{
  size_t count = 0;
  while (true) {
    SocketEndpoint peer_endpoint;
    Socket conn_socket = socket.accept(peer_endpoint);
    if (conn_socket) {
      WeakHandle weak_handle(conn_socket.handle());
      notices.add_notice(weak_handle, { Notice::Inbound, Notice::Outbound },
          handler_creator(std::move(conn_socket), peer_endpoint));
      ++count;
    } else {
      break;
    }
  }
  return count;
}


} } // namespace ku::fusion

