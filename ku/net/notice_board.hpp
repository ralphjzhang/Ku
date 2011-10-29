#pragma once
#include "notice.hpp"

namespace ku { namespace net {

class NoticeBoard
{
public:
  template <typename Handle>
  bool add_notice(Handle const& h, void* event_handler, Notice::Type type,
                  Notice::EventType event_type) // TODO this can be multiple
  {
    Notice notice(h.raw_handle(), type);
    notice.set_event_type(event_type);
    notice.set_event_handler(event_handler);
    return add_notice_internal(std::move(notice));
  }

  bool remove_notice(Notice const& notice) { return remove_notice_internal(notice); }
  bool modify_notice(Notice const& notice);

  NoticeBoard() = default;
  virtual ~NoticeBoard() { };

private:
  virtual bool add_notice_internal(Notice&&) = 0;
  virtual bool remove_notice_internal(Notice const&) = 0;
  virtual bool modify_notice_internal(Notice const&) = 0;
};

} } // namespace ku::net

