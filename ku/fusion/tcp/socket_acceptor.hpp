/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "../notice.hpp"
#include "../notice_board.hpp"
#include "../ip_endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion { namespace tcp {

// =======================================================================================
// Free function accept_connections captures the common procedure of accepting incoming
// connections.
// The 3rd argument, handler_creator, is supposed to be a function returning function, its
// return value will be the event handler of the accepted connection.
// =======================================================================================
size_t accept_connections(AcceptorSocket& socket, NoticeBoard& notices,
    std::function<Notice::EventHandler(Socket&&, IPEndpoint const&)> handler_creator);


template <typename Connection>
class SocketAcceptor
{
public:
  SocketAcceptor(IPEndpoint const& local_endpoint, NoticeBoard& notices)
    : local_endpoint_(local_endpoint), socket_(local_endpoint), notices_(notices)
  {
    notices_.add_notice(socket_.handle(), [this](Notice::Event, NoticeId) { return (*this)(); },
          { Notice::Inbound });
  }

  bool operator ()()
  {
    accept_connections(socket_, notices_, 
        [](Socket&& socket, IPEndpoint const& peer_endpoint) {
          // TODO exception safe
          Connection* conn_ptr = new Connection(std::move(socket), peer_endpoint);
          return Notice::EventHandler([conn_ptr](Notice::Event event, NoticeId id) {
            return (*conn_ptr)(event, id);
            });
        });
    return true;
  }

  AcceptorSocket& socket() { return socket_; }
  IPEndpoint const& local_endpoint() { return local_endpoint; }

private:
  IPEndpoint local_endpoint_;
  AcceptorSocket socket_;
  NoticeBoard& notices_;
};

} } } // namespace ku::fusion::tcp
