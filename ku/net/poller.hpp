#pragma once
#include <sys/epoll.h>
#include <system_error>
#include <vector>
#include <chrono>
#include <ku/util/noncopyable.hpp>

namespace ku { namespace net {

class Channel;
  
namespace poller {

enum class EventsType
{
  None = 0, Read = EPOLLIN, Write = EPOLLOUT
};

inline int to_int(EventsType et) { return static_cast<int>(et); }

class events
{
public:
  events() : count_(0) { }
  events(size_t capacity) : count_(0), events_(capacity) { }
  events(events&& e) { events_ = std::move(e.events_); }

  epoll_event* raw_begin() { return &*events_.begin(); }
  epoll_event const& raw_event(int n) const { return events_[n]; }
 
  int size() const { return static_cast<int>(events_.size()); }
  void resize(size_t size) { events_.resize(size); }

  int count() const { return count_; }
  void set_count(int count) { count_ = count; }

private:
  int count_;
  std::vector<epoll_event> events_;
};

class ChannelList
{
public:
  void add(Channel* ch) { channels_.push_back(ch); }

private:
  std::vector<Channel*> channels_; 
};

class handle : private ku::util::noncopyable
{
public:
  explicit handle(int raw_handle)
    : raw_handle_(raw_handle)
  { }

  template <typename Err>
  handle(int raw_handle, Err err)
    : raw_handle_(raw_handle)
  { set_error(err); }

  handle(handle&& h)
    : raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }

  int raw_handle() const { return raw_handle_; }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }

private:
  int raw_handle_;
  std::error_code error_;
};

handle create(int flags = EPOLL_CLOEXEC);

events& poll(handle& h, events& evts,
    std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));
ChannelList& dispatch(events const& evts, ChannelList& chs);
handle& update(handle& h, int op, Channel const& ch);
handle& add_channel(handle& h, Channel const& ch);
handle& remove_channel(handle& h, Channel const& ch);
handle& modify_channel(handle& h, Channel const& ch);

handle& close(handle& h);

} } } // namespace ku::net::poller

