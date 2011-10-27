#pragma once

namespace ku { namespace net {

class Notice;

class NoticeBoard
{
public:
  virtual bool add_notice(Notice&& notice) = 0;
  virtual bool remove_notice(Notice const& notice) = 0;
  virtual bool modify_notice(Notice const& notice) = 0;

  NoticeBoard() = default;
  virtual ~NoticeBoard() { };
};

} } // namespace ku::net

