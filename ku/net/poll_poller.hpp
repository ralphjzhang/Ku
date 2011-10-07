#pragma once
#include <poll.h>
#include <strings.h>
#include <vector>
#include <system_error>
#include <chrono>
#include <unordered_map>
#include <ku/util/noncopyable.hpp>

namespace ku { namespace net {

class Channel;
class ChannelList;

namespace poll {

enum class EventsType
{
  None = 0, Read = POLLIN, Write = POLLOUT
};

class Events
{
  friend class Poller;
  friend ChannelList& dispatch(Events&, ChannelList&);

public:
  Events() : count_(0) { clear(); }
  Events(size_t capacity) : count_(0), events_(capacity) { clear(); }
  Events(Events&& e) { events_ = std::move(e.events_); }

  pollfd* raw_begin() { return &*events_.begin(); }
  pollfd const& raw_event(int n) const { return events_[n]; }

  int count() const { return count_; }
  void set_count(int count) { count_ = count; }

  void add_channel(Channel& ch);

private:
  int size() const { return static_cast<int>(events_.size()); }
  void resize(size_t size) { events_.resize(size); }

  void clear() { ::bzero(raw_begin(), sizeof(pollfd) * size()); }

  int count_;
  std::vector<pollfd> events_;
  std::unordered_map<int, Channel*> channels_;
};

class Poller : private ku::util::noncopyable
{
private:
  explicit Poller() = default;

public:
  Poller(Poller&& h)
    : error_(h.error_)
  { h.clear(); }

  static Poller create() { return Poller(); }

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { error_.clear(); }

private: 
  std::error_code error_;
};

ChannelList& dispatch(Events& evts, ChannelList& chs);

} // namespace ku::net::poll

} } // namespace ku::net

