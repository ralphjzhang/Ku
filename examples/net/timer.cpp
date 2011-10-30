#include <iostream>
#include <chrono>
#include <thread>
#include <ku/net/notice.hpp>
#include <ku/net/notice_board.hpp>
#include <ku/net/timer.hpp>
#include <ku/net/epoll_poller.hpp>

using namespace ku::net;

struct TimerHandler
{
  epoll::PollLoop& loop;
  Timer periodic_timer, deadline_timer;
  unsigned count;

  TimerHandler(epoll::PollLoop& loop)
    : loop(loop)
  {
    count = 0;
    deadline_timer.set_expires_in(std::chrono::seconds(5));
    periodic_timer.set_interval(std::chrono::seconds(1));

    loop.set_on_initialize([this](NoticeBoard& notice_board) {
        notice_board.add_notice(deadline_timer.handle(),
            [this](Notice::Event, NoticeId) { return handle_deadline_timer(); }, { Notice::Inbound });
        notice_board.add_notice(periodic_timer.handle(), 
            [this](Notice::Event, NoticeId id) { return handle_periodic_timer(id); }, { Notice::Inbound });
        return true;
        });

    // This is optional
    loop.set_on_error([](std::error_code ec) {
        std::cout << "Poller error: " << ec.message() << std::endl;
        return false;
        });
  }

  bool handle_periodic_timer(NoticeId id)
  {
    uint64_t u;
    periodic_timer.read(u, 0);
    std::cout << "Notice " << id << ": Periodic timer ticked " << ++count << " times." << std::endl;
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

  ~TimerHandler() { std::cout << "TimerHandler destroyed." << std::endl; }
};

int main()
{
  std::cout << "Starting event loop." << std::endl;
  epoll::PollLoop loop;
  TimerHandler timer_handler(loop);
  loop();
  std::cout << "Event loop exited." << std::endl;
}

