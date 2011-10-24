#include <cassert>
#include "poll_poller.hpp"
#include "channel.hpp"

namespace ku { namespace net { namespace poll {

/*
// Helper function
int translate_event_types(Channel const& chan)
{
  int event_types = 0;
  if (chan.has_event_type(Channel::In))
    event_types |= (POLLIN | POLLPRI | POLLRDHUP);
  if (chan.has_event_type(Channel::Out))
    event_types |= POLLOUT;
  return event_types;
}

void translate_event_types(int event_types, Channel& chan)
{
  if (event_types & (POLLIN | POLLPRI))
    chan.set_event_type(Channel::In);
  if (event_types & POLLOUT)
    chan.set_event_type(Channel::Out);
}

void translate_events(pollfd const& ev, Channel& chan)
{
  if (ev.revents & (POLLHUP | POLLRDHUP))
    chan.set_event(Channel::Close);
  if (ev.revents & (POLLIN | POLLPRI | POLLRDHUP))
    chan.set_event(Channel::Read);
  if (ev.revents & POLLOUT)
    chan.set_event(Channel::Write);
  if (ev.revents & (POLLERR | POLLNVAL))
    chan.set_event(Channel::Error);
}

/// Events ///

Events::Events(Events&& e)
{
  active_count_ = e.active_count_;
  events_ = std::move(e.events_);
  channels_ = std::move(e.channels_);
  removal_ = std::move(e.removal_);
  e.clear();
}

void Events::clear()
{
  active_count_ = 0;
  events_.clear();
  channels_.clear();
  removal_.clear();
}

bool Events::adopt_channel(Channel&& chan)
{
  assert(chan.any_event_type());
  size_t evt_count = channels_.size();
  int raw_handle = chan.raw_handle();
  auto res = channels_.insert(std::make_pair(raw_handle, 
                                             std::make_pair(std::move(chan), evt_count)));
  if (res.second) {
    if (evt_count >= events_.size())
      resize(evt_count + evt_count / 2);
    pollfd& ev = events_[evt_count];
    Channel* ch_ptr = &res.first->second.first;
    ev.fd = ch_ptr->raw_handle();
    ev.events = translate_event_types(*ch_ptr);
    return true;
  }
  return false;
}

Channel* Events::find_channel(int fd)
{
  auto find = channels_.find(fd);
  return channels_.end() == find ? nullptr : &find->second.first;
}

bool Events::remove_channel(Channel const& chan)
{
  if (find_channel(chan.raw_handle())) {
    removal_.push_back(chan.raw_handle());
    return true;
  }
  return false;
}

bool Events::modify_channel(Channel const& chan)
{
  auto find = channels_.find(chan.raw_handle());
  if (channels_.end() != find) {
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == chan.raw_handle());
    ev.events = translate_event_types(chan);
    find->second.first.set_event_types(chan.event_types());
    return true;
  }
  return false;
}

void Events::apply_removal()
{
  for (int fd : removal_)
    remove_channel_internal(fd);
}

bool Events::remove_channel_internal(int fd)
{
  auto find = channels_.find(fd);
  if (channels_.end() != find) {
    assert(find->second.first.raw_handle() == fd);
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == fd);
    ev.fd = 0;
    channels_.erase(find);
    compress(idx);
    return true;
  }
  return false;
}

void Events::compress(size_t idx)
{
  size_t evt_count = channels_.size();
  if (idx != evt_count) {
    // Move the last pollfd in events_ to the 'hole' in idx
    events_[idx] = events_[evt_count];
    events_[evt_count].fd = 0;
    auto find = channels_.find(events_[idx].fd);
    assert(find != channels_.end());
    find->second.second = idx;
  }
  if (events_.size() > Events::InitialCapacity && evt_count < events_.size() / 2)
    events_.resize(evt_count + evt_count / 2);
}

/// Poller ///

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::poll(evts.raw_events(), evts.channels_.size(), timeout.count());
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

