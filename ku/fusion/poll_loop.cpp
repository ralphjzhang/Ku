/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include "notice.hpp"
#include "poll_loop.hpp"

namespace ku { namespace fusion {

void PollLoop::dispatch(Notice& notice, NoticeBoard& notice_board)
{
  Notice::EventHandler& event_handler = notice.event_handler();
  // Read
  if (notice.has_event(Notice::Read))
    event_handler(Notice::Read, notice.id());
  // Write
  if (notice.has_event(Notice::Write))
    event_handler(Notice::Write, notice.id());
  // Error
  if (notice.has_event(Notice::Error))
    if (!event_handler(Notice::Error, notice.id()))
      notice_board.remove_notice(notice.id());
  // Close
  if (notice.has_event(Notice::Close)) {
    event_handler(Notice::Close, notice.id());
    notice_board.remove_notice(notice.id());
  }
}

} } // namespace ku::fusion

