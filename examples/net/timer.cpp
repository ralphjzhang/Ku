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
  unsigned count;

  bool initialize(NoticeBoard& notice_board)
  {
    count = 0;
    periodic_timer.set_interval(std::chrono::seconds(1));
    notice_board.add_notice(periodic_timer.handle(), this, Notice::Connection, Notice::Inbound);
    notice_board.add_notice(deadline_timer.handle(), this, Notice::Connection, Notice::Inbound);
    return true;
  }

  bool on_error(std::error_code) { return false; }

  void dispatch(Notice& notice, NoticeBoard& notice_board)
  {
    ku::net::dispatch<TimerDispatcher, TimerDispatcher>(notice, notice_board);
  }

  bool handle_accept(NoticeBoard&) { return true; }

  bool handle_read()
  {
    uint64_t u;
    periodic_timer.read(u, 0);
    std::cout << "Timer ticked " << ++count << " times." << std::endl;
    //if (count == 4)
    //  quit = true;
    return true;
  }

  ~TimerDispatcher() { std::cout << "TimerDispatcher destroyed." << std::endl; }
};

int main()
{
  std::cout << "Starting event loop." << std::endl;
  TimerDispatcher timer_dispatcher;
  epoll::PollLoop<TimerDispatcher> loop;
  if (!loop(timer_dispatcher))
    std::cout << loop.error().message() << std::endl;
  std::cout << "Event loop exited." << std::endl;
}

