#include <cassert>
#include "poll_poller.hpp"
#include "notice.hpp"

namespace ku { namespace fusion { namespace poll {

// Helper function
int translate_event_types(Notice const& notice)
{
  int event_types = 0;
  if (notice.has_event_type(Notice::Inbound))
    event_types |= (POLLIN | POLLPRI | POLLRDHUP);
  if (notice.has_event_type(Notice::Outbound))
    event_types |= POLLOUT;
  return event_types;
}

void translate_event_types(int event_types, Notice& notice)
{
  if (event_types & (POLLIN | POLLPRI))
    notice.set_event_type(Notice::Inbound);
  if (event_types & POLLOUT)
    notice.set_event_type(Notice::Outbound);
}

void translate_events(pollfd const& ev, Notice& notice)
{
  if (ev.revents & (POLLHUP | POLLRDHUP))
    notice.set_event(Notice::Close);
  if (ev.revents & (POLLIN | POLLPRI | POLLRDHUP))
    notice.set_event(Notice::Read);
  if (ev.revents & POLLOUT)
    notice.set_event(Notice::Write);
  if (ev.revents & (POLLERR | POLLNVAL))
    notice.set_event(Notice::Error);
}

/// Events ///
//
void Events::clear()
{
  active_count_ = 0;
  events_.clear();
  notices_.clear();
}

Notice* Events::find_notice(pollfd const& ev)
{
  // TODO
}

bool Events::add_notice_internal(Notice&& notice)
{
  assert(notice.any_event_type());
  size_t evt_count = notices_.size();
  int raw_handle = notice.raw_handle();
  auto res = notices_.insert(std::make_pair(raw_handle,
                                            std::make_pair(std::move(notice), evt_count)));
  if (res.second) {
    if (evt_count >= events_.size())
      resize(evt_count + evt_count / 2);
    pollfd& ev = events_[evt_count];
    Notice* ch_ptr = &res.first->second.first;
    ev.fd = ch_ptr->raw_handle();
    ev.events = translate_event_types(*ch_ptr);
    return true;
  }
  return false;
}

bool Events::remove_notice_internal(NoticeId id)
{
  auto find = notices_.find(id);
  if (notices_.end() != find) {
    assert(find->second.first.id() == id);
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    ev.fd = 0;
    notices_.erase(find);
    compress(idx);
    return true;
  }
  return false;
}

bool Events::modify_notice_internal(NoticeId id, Notice const& notice)
{
  auto find = notices_.find(id);
  if (notices_.end() != find) {
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == notice.raw_handle());
    ev.events = translate_event_types(notice);
    find->second.first.set_event_types(notice.event_types());
    find->second.first.set_event_handler(notice.event_handler());
    return true;
  }
  return false;
}

Notice* Events::find_notice(NoticeId id)
{
  auto find = notices_.find(id);
  return notices_.end() == find ? nullptr : &find->second.first;
}

void Events::compress(size_t idx)
{
  size_t evt_count = notices_.size();
  if (idx != evt_count) {
    // Move the last pollfd in events_ to the 'hole' in idx
    events_[idx] = events_[evt_count];
    events_[evt_count].fd = 0;
    auto find = notices_.find(events_[idx].fd);
    assert(find != notices_.end());
    find->second.second = idx;
  }
  if (events_.size() > Events::InitialCapacity && evt_count < events_.size() / 2)
    events_.resize(evt_count + evt_count / 2);
}

/// Poller ///
//
Events& poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::poll(evts.raw_events(), evts.notices_.size(), timeout.count());
  if (event_num == -1) { // TODO EINTR
    evts.set_active_count(0);
    throw std::system_error(util::errc(), "poll::poll");
  } else {
    evts.set_active_count(util::implicit_cast<unsigned>(event_num));
  }
  return evts;
}

/// PollLoop ///
//
bool PollLoop::loop(std::chrono::milliseconds timeout)
{
  while (!quit_) {
    events_.apply_updates();
    poll(events_, timeout);
  
    for (unsigned i = 0; i < events_.events_count(); ++i) {
      pollfd const& ev = events_.raw_event(i);
      if (ev.revents == 0)
        continue;
      Notice* notice = events_.find_notice(ev);
      translate_events(ev, *notice);
      dispatch(*notice, events_);
    }
  }
  return true;
}

} } } // namespace ku::fusion::poll

