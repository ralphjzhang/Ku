/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "notice_board.hpp"

namespace ku { namespace fusion {

class PollLoop
{
public:
  PollLoop() : quit_(false) { }

  void quit() { quit_ = true; } // TODO quit should take care of timeout=-1
  virtual NoticeBoard& notices() = 0;

  bool operator () (std::chrono::milliseconds timeout = std::chrono::milliseconds(-1))
  { return loop(timeout); }

private:
  virtual bool loop(std::chrono::milliseconds timeout) = 0;

protected:
  void dispatch(Notice& notice, NoticeBoard& notice_board);

  bool quit_;
};

} } // namespace ku::fusion

