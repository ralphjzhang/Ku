#include <cassert>
#include "poll_poller.hpp"
#include "notice.hpp"

namespace ku { namespace net { namespace poll {

/*
// Helper function
int translate_event_types(Notice const& notice)
{
  int event_types = 0;
  if (notice.has_event_type(Notice::In))
    event_types |= (POLLIN | POLLPRI | POLLRDHUP);
  if (notice.has_event_type(Notice::Out))
    event_types |= POLLOUT;
  return event_types;
}

void translate_event_types(int event_types, Notice& notice)
{
  if (event_types & (POLLIN | POLLPRI))
    notice.set_event_type(Notice::In);
  if (event_types & POLLOUT)
    notice.set_event_type(Notice::Out);
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

Events::Events(Events&& e)
{
  active_count_ = e.active_count_;
  events_ = std::move(e.events_);
  notices_ = std::move(e.notices_);
  removal_ = std::move(e.removal_);
  e.clear();
}

void Events::clear()
{
  active_count_ = 0;
  events_.clear();
  notices_.clear();
  removal_.clear();
}

bool Events::adopt_notice(Notice&& notice)
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

Notice* Events::find_notice(int fd)
{
  auto find = notices_.find(fd);
  return notices_.end() == find ? nullptr : &find->second.first;
}

bool Events::remove_notice(Notice const& notice)
{
  if (find_notice(notice.raw_handle())) {
    removal_.push_back(notice.raw_handle());
    return true;
  }
  return false;
}

bool Events::modify_notice(Notice const& notice)
{
  auto find = notices_.find(notice.raw_handle());
  if (notices_.end() != find) {
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == notice.raw_handle());
    ev.events = translate_event_types(notice);
    find->second.first.set_event_types(notice.event_types());
    return true;
  }
  return false;
}

void Events::apply_removal()
{
  for (int fd : removal_)
    remove_notice_internal(fd);
}

bool Events::remove_notice_internal(int fd)
{
  auto find = notices_.find(fd);
  if (notices_.end() != find) {
    assert(find->second.first.raw_handle() == fd);
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == fd);
    ev.fd = 0;
    notices_.erase(find);
    compress(idx);
    return true;
  }
  return false;
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

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::poll(evts.raw_events(), evts.notices_.size(), timeout.count());
  if (event_num == -1) {
    set_error(errno);
    evts.set_active_count(0);
  } else {
    evts.set_active_count(util::implicit_cast<unsigned>(event_num));
  }
  return evts;
}
*/

} } } // namespace ku::net::poll

