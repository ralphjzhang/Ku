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
  epoll::PollLoop<TimerDispatcher> loop;
  Timer periodic_timer, deadline_timer;
  unsigned count;

  bool initialize(NoticeBoard& notice_board)
  {
    count = 0;
    deadline_timer.set_expires_in(std::chrono::seconds(5));
    periodic_timer.set_interval(std::chrono::seconds(1));

    notice_board.add_notice(deadline_timer.handle(),
        [this](Notice::Event) { return handle_deadline_timer(); }, { Notice::Inbound });
    notice_board.add_notice(periodic_timer.handle(), 
        [this](Notice::Event e) { return handle_periodic_timer(e); }, { Notice::Inbound });
    return true;
  }

  void dispatch(Notice& notice, NoticeBoard& notice_board)
  {
    ku::net::dispatch(notice, notice_board);
  }

  bool handle_periodic_timer(Notice::Event)
  {
    uint64_t u;
    periodic_timer.read(u, 0);
    std::cout << "Periodic timer ticked " << ++count << " times." << std::endl;
    return true;
  }

  bool handle_deadline_timer()
  {
    uint64_t u;
    deadline_timer.read(u, 0);
    std::cout << "Deadline timer ticks, exit." << std::endl;
    loop.quit();
    return true;
  }

  ~TimerDispatcher() { std::cout << "TimerDispatcher destroyed." << std::endl; }
};

int main()
{
  std::cout << "Starting event loop." << std::endl;
  TimerDispatcher timer_dispatcher;
  if (!timer_dispatcher.loop(timer_dispatcher))
    std::cout << timer_dispatcher.loop.error().message() << std::endl;
  std::cout << "Event loop exited." << std::endl;
}

