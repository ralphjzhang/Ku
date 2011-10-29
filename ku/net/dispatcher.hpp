#pragma once
#include <system_error>
#include <functional>
#include "call_traits.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class NoticeBoard;

template <typename Connection, typename Acceptor>
void dispatch(Notice& notice, NoticeBoard& notice_board)
{
  // Accept
  if (notice.type() == Notice::Acceptor && notice.has_event(Notice::Read))
      notice.event_handler<Acceptor>().handle_accept(notice_board);
  dispatch<Connection>(notice, notice_board);
}

template <typename Connection>
void dispatch(Notice& notice, NoticeBoard& notice_board)
{
  Connection& connection = notice.event_handler<Connection>();
  // Read
  if (notice.has_event(Notice::Read))
    util::if_handle_read(connection);
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

// =======================================================================================
// A default dispatcher does nothing. Client code can choose to inherit from this class, or
// provide a complete implementation from scratch. A dispatcher must have following operations:
//
//   bool initialize(NoticeBoard&);
//   bool on_error(std::error_code);
//   void dispatch(Notice&, NoticeBoard&);
//
// It is advised that dispatch calls the template methods dispatch<>, but client can
// also choose to implement his own dispatching merchanism.
// =======================================================================================
template <typename EventHandler>
class Dispatcher
{
public:
  Dispatcher() = default;

  // Dispatcher requirements
  bool initialize(NoticeBoard&) { return true; }
  bool on_error(std::error_code) { return false; }
  void dispatch(Notice& notice, NoticeBoard& notice_board) { }
};

} } // namespace ku::net

