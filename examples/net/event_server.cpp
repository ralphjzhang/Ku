#include <iostream>
#include <chrono>
#include <ku/net/notice.hpp>
#include <ku/net/notice_board.hpp>
#include <ku/net/user_event.hpp>
#include <ku/net/dispatcher.hpp>
#include <ku/net/epoll_poller.hpp>

using namespace ku::net;

struct UserEventDispatcher
{
  UserEventDispatcher() : user_event(42), value(0) { }

  UserEvent user_event;

  bool initialize(NoticeBoard& notice_board)
  {
    notice_board.add_notice(user_event.handle(), this, Notice::Connection, Notice::Inbound);
    return true;
  }

  bool on_error(std::error_code) { return false; }

  void dispatch(Notice& notice, NoticeBoard& notice_board)
  {
    ku::net::dispatch<UserEventDispatcher, UserEventDispatcher>(notice, notice_board);
  }

  bool handle_accept(NoticeBoard&) { return true; }

  bool handle_inbound()
  {
    if (value++ < 10) {
      uint64_t u = 0;
      //::read(user_event.raw_handle(), &u, sizeof(u));
      //::write(user_event.raw_handle(), &value, sizeof(value));
    } else {
      //quit = true;
    }
    return true;
  }
  uint64_t value;
};

int main()
{
  UserEventDispatcher dispatcher;
  if (std::error_code ec = epoll::poll_loop(dispatcher))
    std::cout << ec.message() << std::endl;
  std::cout << "Event loop exited." << std::endl;
}

