#include <iostream>
#include <chrono>
#include <thread>
#include <ku/net/notice.hpp>
#include <ku/net/notice_board.hpp>
#include <ku/net/timer.hpp>
#include <ku/net/dispatcher.hpp>
#include <ku/net/epoll_poller.hpp>

using namespace ku::net;

struct TimerDispatcher
{
  Timer periodic_timer, deadline_timer;
  bool quit;
  unsigned count;

  bool initialize(NoticeBoard& notice_board)
  {
    count = 0;
    auto add_timer = [&](Timer const& timer) {
      Notice notice(timer.raw_handle(), Notice::Timer);
      notice.set_event_type(Notice::In);
      notice.set_event_handler(this);
      notice_board.add_notice(std::move(notice));
    };
    periodic_timer.set_interval(std::chrono::seconds(1));
    deadline_timer.set_expires_in(std::chrono::seconds(5));
    add_timer(periodic_timer);
    add_timer(deadline_timer);
    return true;
  }

  bool on_error(std::error_code) { return false; }

  void dispatch(Notice& notice, NoticeBoard& notice_board)
  {
    ku::net::dispatch<TimerDispatcher, TimerDispatcher, TimerDispatcher>(notice, notice_board);
  }

  bool get_quit() const { return quit; }

  bool handle_accept(NoticeBoard&) { return true; }

  bool handle_timer_tick()
  {
    std::cout << "Timer ticked " << ++count << " times." << std::endl;
    if (count == 4)
      quit = true;
    return true;
  }

  ~TimerDispatcher() { std::cout << "TimerDispatcher destroyed." << std::endl; }
};

int main()
{
  std::cout << "Starting event loop." << std::endl;
  TimerDispatcher timer_dispatcher;
  if (std::error_code ec = epoll::poll_loop(timer_dispatcher))
    std::cout << ec.message() << std::endl;
  std::cout << "Event loop exited." << std::endl;
}

