/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include <functional>
#include "socket.hpp"

namespace ku { namespace net {

class NoticeBoard;

void dispatch(Notice& notice, NoticeBoard& notice_board)
{
  auto& event_handler = notice.event_handler();
  // Read
  if (notice.has_event(Notice::Read))
    event_handler(Notice::Read);
  // Write
  if (notice.has_event(Notice::Write))
    event_handler(Notice::Write);
  // Error
  if (notice.has_event(Notice::Error))
    if (!event_handler(Notice::Error))
      notice_board.remove_notice(notice);
  // Close
  if (notice.has_event(Notice::Close)) {
    event_handler(Notice::Close);
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

