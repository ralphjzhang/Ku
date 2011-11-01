/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "notice_board.hpp"
#include "socket_acceptor.hpp"

namespace ku { namespace net {

// TODO by customizing this, we can suppose multiple models:
//      reactor (this one is)
//      thread per connection
//      many threads / many connections
size_t accept_connections(AcceptorSocket& socket, NoticeBoard& notices,
    std::function<Notice::EventHandler(StreamSocket&&, Endpoint const&)> handler_creator)
{
  size_t count = 0;
  while (true) {
    Endpoint peer_endpoint;
    StreamSocket conn_socket = socket.accept(peer_endpoint);
    if (!socket.error()) {
      WeakHandle weak_handle(conn_socket.handle());
      notices.add_notice(weak_handle, handler_creator(std::move(conn_socket), peer_endpoint),
          { Notice::Inbound, Notice::Outbound });
      ++count;
    } else {
      std::error_code const& ec = socket.error();
      if (ec == std::errc::operation_would_block ||
          ec == std::errc::resource_unavailable_try_again) {
        // All incoming connections handled, not an error
        socket.clear_error();
      }
      // else: Acceptor error, keep the error for caller inspection
      break;
    }
  }
  return count;
}


} } // namespace ku::net

