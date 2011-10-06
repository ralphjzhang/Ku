#pragma once
#include <sys/epoll.h>
#include <system_error>
#include <chrono>
#include <ku/util/noncopyable.hpp>

namespace ku { namespace net {

class Channel;
class ChannelList;
  
namespace epoll {

enum class EventsType
{
  None = 0, Read = EPOLLIN, Write = EPOLLOUT
};

inline int to_int(EventsType et) { return static_cast<int>(et); }

class Event
{
  friend class Events;
  Event(epoll_event const* event) : event_(event) { }

  epoll_event const* event_;

public:
  bool connect() const;
  bool read() const;
  bool write() const;
};


class Events
{
public:
  typedef Event* iterator;
  typedef Event const* const_iterator;

  Events() : count_(0) { clear(); }
  Events(size_t capacity) : count_(0), events_(capacity) { clear(); }
  Events(Events&& e) { events_ = std::move(e.events_); }

  const_iterator begin() const { return iterator(raw_begin()); }
  const_iterator end() const { return iterator(nullptr); }

  epoll_event* raw_begin() { return &*events_.begin(); }
  epoll_event const* raw_begin() const { return &*events_.cbegin(); }
  epoll_event const& raw_event(int n) const { return events_[n]; }
 
  int size() const { return static_cast<int>(events_.size()); }
  void resize(size_t size) { events_.resize(size); }

  int count() const { return count_; }
  void set_count(int count) { count_ = count; }

private:
  void clear() { ::bzero(raw_begin(), sizeof(epoll_event) * size()); }

  int count_;
  std::vector<epoll_event> events_;
};

class Poller : private ku::util::noncopyable
{
private:
  explicit Poller(int raw_handle)
    : raw_handle_(raw_handle)
  { }

  template <typename Err>
  Poller(int raw_handle, Err err)
    : raw_handle_(raw_handle)
  { set_error(err); }

public:
  Poller(Poller&& h)
    : raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }

  static Poller create(int flags = EPOLL_CLOEXEC);
  int raw_handle() const { return raw_handle_; }

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  Poller& update(int op, Channel const& ch);
  Poller& add_channel(Channel const& ch);
  Poller& remove_channel(Channel const& ch);
  Poller& modify_channel(Channel const& ch);

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }

private:
  int raw_handle_;
  std::error_code error_;
};

ChannelList& dispatch(Events const& evts, ChannelList& chs);

Poller& close(Poller& h);

} } } // namespace ku::net::epoll

