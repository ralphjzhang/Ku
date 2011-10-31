/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include <iostream>
#include "resolver.hpp"
#include "endpoint.hpp"
#include "socket.hpp"
#include "notice.hpp"
#include "notice_board.hpp"

namespace ku { namespace net {

// =======================================================================================
// Free function accept_connections captures the common procedure of accepting incoming
// connections.
// The 3rd argument, handler_creator, is supposed to be a function returning function, its
// return value will be the event handler of the accepted connection.
// =======================================================================================
size_t accept_connections(AcceptorSocket& socket, NoticeBoard& notices,
    std::function<Notice::EventHandler(StreamSocket&&, Endpoint const&)> handler_creator);


template <typename Connection>
class Acceptor
{
public:
  Acceptor(Endpoint const& endpoint, NoticeBoard& notices)
    : endpoint_(endpoint), socket_(endpoint), notices_(notices)
  {
    if (!socket_.error()) {
      using namespace std::placeholders;
      notices_.add_notice(socket_.handle(), [this](Notice::Event, NoticeId) { (*this)(); },
          { Notice::Inbound });
    }
  }

  bool operator ()()
  {
    accept_connections(socket_, notices_, 
        [](StreamSocket&& socket, Endpoint const& peer_endpoint) {
          // TODO exception safe
          Connection* conn_ptr = new Connection(std::move(socket), peer_endpoint);
          return Notice::EventHandler([conn_ptr](Notice::Event event, NoticeId id) {
            *conn_ptr(event, id);
            });
        });
    return !socket_.error();
  }

  AcceptorSocket& socket() { return socket_; }

private:
  Endpoint endpoint_;
  AcceptorSocket socket_;
  NoticeBoard& notices_;
};

} } // namespace ku::net

