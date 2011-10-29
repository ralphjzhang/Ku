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
#include "dispatcher.hpp"

namespace ku { namespace net {

template <typename EventHandler>
class Acceptor
{
public:
  Acceptor(Endpoint const& endpoint)
    : endpoint_(endpoint), acceptor_socket_(endpoint) { }

  bool handle_accept_error(AcceptorSocket const& sock)
  {
    return true;
  }

  // Acceptor requirement
  void handle_accept(NoticeBoard& notice_board)
  {
    while (true) {
      Endpoint peer_endpoint;
      StreamSocket conn_socket = acceptor_socket_.accept(peer_endpoint);
      if (!acceptor_socket_.error()) {
        std::cout << "Connection from: " << to_str(peer_endpoint) << std::endl;
        WeakHandle weak_handle(conn_socket.handle());
        /*
        notice_board.add_notice(
            weak_handle, new EventHandler(std::move(conn_socket), peer_endpoint),
            Notice::Inbound); // TODO also need outbound
            */
      } else {
        std::error_code ec = acceptor_socket_.error();
        if (ec == std::errc::operation_would_block ||
            ec == std::errc::resource_unavailable_try_again) {
          // All incoming connections handled
          break;
        } else {
          // Acceptor error
          handle_accept_error(acceptor_socket_);
          break;
        }
      }
    }
  }

  // Dispatcher requirement
  void dispatch(Notice& notice, NoticeBoard& notice_board)
  {
    return ku::net::dispatch(notice, notice_board);
  }

  bool initialize(NoticeBoard& notice_board)
  {
    if (acceptor_socket_.error()) {
      std::cout << "Listener error: " << acceptor_socket_.error().message() << std::endl;
      exit(0);
    }
    using namespace std::placeholders;
    notice_board.add_notice(acceptor_socket_.handle(),
        std::bind(&Acceptor<EventHandler>::handle_accept, this, _1), { Notice::Inbound });
    return true;
  }

  bool on_error(std::error_code ec)
  {
    std::cout << "Poller error: " << ec.message() << std::endl;
    return false;
  }

private:
  Endpoint endpoint_;
  AcceptorSocket acceptor_socket_;
};

} } // namespace ku::net

