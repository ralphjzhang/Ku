#pragma once
#include <system_error>
#include <functional>
#include "handle_ops.hpp"
#include "notice_ops.hpp"
#include "call_traits.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class NoticeBoard;

template <typename EventHandler, typename Acceptor>
void accept_connections(Acceptor& acceptor, Notice& notice, NoticeBoard& notice_board)
{
  /*
  while (true) {
    Endpoint endpoint;
    AcceptorSocket acceptor_socket(notice.handle());
    StreamSocket conn_socket = acceptor_socket.accept(endpoint);
    if (acceptor_socket.error()) {
      if (acceptor_socket.error() == std::errc::operation_would_block ||
          acceptor_socket.error() == std::errc::resource_unavailable_try_again) {
        // All incoming connections handled
        break;
      } else {
        // Acceptor error
        if (!util::if_handle_error(acceptor, acceptor_socket))
          notice_board.remove_notice(notice);
        break;
      }
    } else {
      if (util::if_handle_accept(acceptor, conn_socket, endpoint)) {
        Notice conn_notice;
        conn_notice.adopt(std::move(conn_socket));
        conn_notice.set_event_type(Notice::In);
        conn_notice.set_event_handler(std::make_shared<EventHandler>());
        notice_board.adopt_notice(std::move(conn_notice));
      }
    }
  }
  */
}

template <typename Acceptor, typename Connection>
void dispatch(Notice& notice, NoticeBoard& notice_board)
{
  Connection& connection = notice.event_handler<Connection>();

  // Read
  if (notice.has_event(Notice::Read)) {
    if (notice.type() == Notice::Acceptor) {
      // Accept
      notice.event_handler<Acceptor>().handle_accept(notice_board);
    } else if (notice.type() == Notice::Connection) {
      // Data read
      util::if_handle_read(connection);
    }
  }
  // Write
  if (notice.has_event(Notice::Write))
    util::if_handle_write(connection);
  // Error
  if (notice.has_event(Notice::Error))
    if (!util::if_handle_error(connection))
      notice_board.remove_notice(notice);
  // Close
  if (notice.has_event(Notice::Close)) {
    util::if_handle_close(connection);
    notice_board.remove_notice(notice);
  }
}

template <typename Acceptor, typename Connection, typename TimerHandler>
void dispatch(Notice& notice, NoticeBoard& notice_board)
{
  // Timer
  if (notice.type() == Notice::Timer && notice.has_event(Notice::Read)) {
    int64_t tick;
    ::read(notice.raw_handle(), &tick, sizeof(tick));
    TimerHandler& timer_handler = notice.event_handler<TimerHandler>();
    if (!util::if_handle_timer_tick(timer_handler))
      notice_board.remove_notice(notice);
  }
  dispatch<Acceptor, Connection>(notice, notice_board);
}

// =======================================================================================
// A default dispatcher does nothing. Client code can choose to inherit from this class, or
// provide a complete implementation from scratch. A dispatcher must have following operations:
//
//   bool initialize(Noticenotice_board&);
//   bool on_error(std::error_code);
//   bool get_quit() const;
//   void dispatch(Notice&, Noticenotice_board&);
//
// It is advised that dispatch calls the template methods dispatch<>, but client can
// also choose to implement his own dispatching merchanism.
// =======================================================================================
template <typename EventHandler>
class Dispatcher
{
public:
  Dispatcher() : quit_(false) { }

  // Dispatcher requirements
  bool initialize(NoticeBoard&) { return true; }
  bool on_error(std::error_code) { return false; }
  bool get_quit() const { return quit_; }
  void dispatch(Notice& notice, NoticeBoard& notice_board) { }

  void quit() { quit_ = true; }

private:
  bool quit_;
};

} } // namespace ku::net

