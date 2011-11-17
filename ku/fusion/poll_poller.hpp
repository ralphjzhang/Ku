#pragma once
#include <poll.h>
#include <vector>
#include <system_error>
#include <chrono>
#include <unordered_map>
#include "notice.hpp"
#include "notice_board.hpp"
#include "poll_loop.hpp"

namespace ku { namespace fusion { namespace poll {

class Events : public NoticeBoard
             , private util::noncopyable
{
  friend Events& poll(Events&, std::chrono::milliseconds const&);
  static const size_t InitialCapacity = 16;

  typedef std::vector<pollfd> EventList;
  // Mapping NoticeId --> (Notice, index in events_)
  typedef std::unordered_map<NoticeId, std::pair<Notice, size_t> > NoticeMap;

public:
  Events(size_t capacity = InitialCapacity) : events_(capacity) { clear(); }
  virtual ~Events() { }

  pollfd const& raw_event(unsigned n) const { return events_[n]; }
  unsigned active_count() const { return active_count_; }
  unsigned events_count() const { return notices_.size(); }

  Notice* find_notice(pollfd const& ev);

  using NoticeBoard::apply_updates;

private:
  virtual bool add_notice_internal(Notice&& notice);
  virtual bool remove_notice_internal(NoticeId id);
  virtual bool modify_notice_internal(NoticeId id, Notice const& notice);
  virtual Notice* find_notice(NoticeId id);

  bool remove_notice_internal(int fd);

  pollfd* raw_events() { return &*events_.begin(); }
  void set_active_count(unsigned n) { active_count_ = n; }

  void clear();
  void compress(size_t idx);
  void resize(size_t size) { events_.resize(size); }

private:
  unsigned active_count_;
  EventList events_;
  NoticeMap notices_;
};

void translate_events(pollfd const& ev, Notice& notice);

// =======================================================================================
// poll::PollLoop is the main loop using poll
// =======================================================================================
class PollLoop : public fusion::PollLoop
{
public:
  PollLoop() { }
  PollLoop(PollLoop const&) = default;

  virtual NoticeBoard& notices() { return events_; }

private:
  virtual bool loop(std::chrono::milliseconds timeout);

private:
  Events events_;
};

} } } // namespace ku::fusion::poll

