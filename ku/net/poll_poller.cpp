#include <cassert>
#include <strings.h>
#include <ku/util/cast.hpp>
#include "poll_poller.hpp"
#include "channel.hpp"

namespace ku { namespace net { namespace poll {

// Helper function
int translate_events_type(Channel::EventsType et)
{
  int events_type = 0;
  if (et | Channel::In)
    events_type |= (POLLIN | POLLPRI);
  if (et | Channel::Out)
    events_type |= POLLOUT;
  return events_type;
}

//////////////
/// Poller ///
//////////////

Events& Poller::poll(Events& evts, std::chrono::milliseconds const& timeout)
{
  int event_num = ::poll(evts.raw_begin(), evts.channels_.size(), timeout.count());
  if (event_num == -1) {
    set_error(errno);
    evts.set_active_count(0);
  }
  evts.set_active_count(implicit_cast<unsigned>(event_num));
  return evts;
}

//////////////
/// Events ///
//////////////

Events::Events(Events&& e)
{
  active_count_ = e.active_count_;
  events_ = std::move(e.events_);
  channels_ = std::move(e.channels_);
  e.active_count_ = 0;
  e.events_.clear();
  e.channels_.clear();
}

void Events::clear()
{
  active_count_ = 0;
  ::bzero(raw_begin(), sizeof(pollfd) * events_.size());
}

bool Events::add_channel(Channel&& ch)
{
  size_t evt_count = channels_.size();
  auto res = channels_.insert(std::make_pair(ch.raw_handle(), std::make_pair(ch, evt_count)));
  if (res.second) {
    if (evt_count >= events_.size())
      resize(evt_count + evt_count / 2);
    pollfd& ev = events_[evt_count];
    ev.fd = ch.raw_handle();
    ev.events = translate_events_type(ch.events_type());
    return true;
  }
  return false;
}

Channel* Events::find_channel(int fd)
{
  auto find = channels_.find(fd);
  return channels_.end() == find ? nullptr : &find->second.first;
}

bool Events::remove_channel(int fd)
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

bool Events::modify_channel(int fd, int events_type)
{
  auto find = channels_.find(fd);
  if (channels_.end() != find) {
    size_t idx = find->second.second;
    assert(0 <= idx && idx < events_.size());
    pollfd& ev = events_[idx];
    assert(ev.fd == fd);
    ev.events = events_type;
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

void translate_events(pollfd const& ev, Channel& ch)
{
  if ((ev.revents & POLLHUP) && !(ev.revents & POLLIN))
    ch.set_event<Channel::Close>();
  if (ev.revents & (POLLIN | POLLPRI | POLLRDHUP))
    ch.set_event<Channel::Read>();
  if (ev.revents & POLLOUT)
    ch.set_event<Channel::Write>();
  if (ev.revents & (POLLERR | POLLNVAL))
    ch.set_event<Channel::Error>();
}

} } } // namespace ku::net::poll

