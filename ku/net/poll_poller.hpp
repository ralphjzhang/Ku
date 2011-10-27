#pragma once
#include <poll.h>
#include <vector>
#include <system_error>
#include <chrono>
#include <unordered_map>
#include "notice.hpp"
#include "notice_board.hpp"

namespace ku { namespace net { namespace poll {

class Events : public NoticeBoard
             , private util::noncopyable
{
  friend class Poller;
  static const size_t InitialCapacity = 16;

  typedef std::vector<pollfd> EventList;
  // Mapping NoticeId --> (Notice, index in events_)
  typedef std::unordered_map<NoticeId, std::pair<Notice, size_t> > NoticeMap;

public:
  Events() : events_(Events::InitialCapacity) { clear(); }
  Events(size_t capacity) : events_(capacity) { clear(); }
  Events(Events&& e);
  virtual ~Events() { }

  pollfd const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }
  unsigned events_count() const { return notices_.size(); }

  virtual bool add_notice(Notice&& notice);
  virtual bool remove_notice(Notice const& notice);
  virtual bool modify_notice(Notice const& notice);
  Notice* find_notice(int fd);

  void apply_removal(); // TODO open this or not?

private:
  bool remove_notice_internal(int fd);

  pollfd* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void compress(size_t idx);
  void resize(size_t size) { events_.resize(size); }

  unsigned active_count_;
  EventList events_;
  NoticeMap notices_;
  std::vector<int> removal_;
};


/**
 * poll::Poller wraps system call ::poll
 * It's supposed to be used with poll::Events
 **/
class Poller : private util::noncopyable
{
private:
  Poller() = default;

public:
  Poller(Poller&& h) : error_(h.error_) { h.clear(); }
  ~Poller() = default;

  static Poller create() { return Poller(); }

  Events& poll(Events& evts,
      std::chrono::milliseconds const& timeout = std::chrono::milliseconds(-1));

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }
  void clear_error() { error_.clear(); }

  void clear() { error_.clear(); }
  void close() { clear(); }

private: 
  std::error_code error_;
};

void translate_events(pollfd const& ev, Notice& ch);

template <typename Dispatcher>
std::error_code poll_loop(Dispatcher& dispatcher,
    std::chrono::milliseconds timeout = std::chrono::milliseconds(3000))
{
  Poller poller = Poller::create();
  Events events;
  dispatcher.initialize(events);

  while (!dispatcher.get_quit()) {
    poller.poll(events, timeout);
    if (poller.error()) {
      if (dispatcher.on_error(poller.error()))
        poller.clear_error();
      else
        return poller.error();
    }

    for (unsigned i = 0; i < events.events_count(); ++i) {
      pollfd const& ev = events.raw_event(i);
      if (ev.revents == 0)
        continue;
      Notice* ch = events.find_notice(ev.fd);
      translate_events(ev, *ch);
      dispatcher.dispatch(*ch, events);
    }
    events.apply_removal();
  }
  return std::error_code();
}

} } } // namespace ku::net::poll

