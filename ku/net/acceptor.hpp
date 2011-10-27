#pragma once
#include <system_error>
#include <memory>
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
    : endpoint_(endpoint), quit_(false), acceptor_socket_(endpoint) { }

  bool handle_accept_error(AcceptorSocket const& sock)
  {
    return true;
  }

  void quit() { quit_ = true; }

  // Acceptor requirement
  void handle_accept(NoticeBoard& notice_board)
  {
    while (true) {
      Endpoint peer_endpoint;
      StreamSocket conn_socket = acceptor_socket_.accept(peer_endpoint);
      if (!acceptor_socket_.error()) {
        std::cout << "Connection from: " << to_str(peer_endpoint) << std::endl;
        Notice conn_notice(conn_socket.raw_handle(), Notice::Connection);
        conn_notice.set_event_type(Notice::In);
        conn_notice.set_event_handler(new EventHandler(std::move(conn_socket), peer_endpoint));
        notice_board.add_notice(std::move(conn_notice));
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
    return ku::net::dispatch<Acceptor<EventHandler>, EventHandler>(notice, notice_board);
  }

  bool get_quit() const { return quit_; }

  bool initialize(NoticeBoard& notice_board)
  {
    if (acceptor_socket_.error()) {
      std::cout << "Listener error: " << acceptor_socket_.error().message() << std::endl;
      exit(0);
    }
    Notice notice(acceptor_socket_.raw_handle(), Notice::Acceptor);
    notice.set_event_type(Notice::In);
    notice.set_event_handler(this);
    notice_board.add_notice(std::move(notice));
    return true;
  }

  bool on_error(std::error_code ec)
  {
    std::cout << "Poller error: " << ec.message() << std::endl;
    return false;
  }

private:
  Endpoint endpoint_;
  bool quit_;
  AcceptorSocket acceptor_socket_;
};

} } // namespace ku::net

