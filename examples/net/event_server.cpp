#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <limits>
#include <ku/net/notice.hpp>
#include <ku/net/notice_board.hpp>
#include <ku/net/user_event.hpp>
#include <ku/net/epoll_poller.hpp>

using namespace ku::net;

bool print_error(std::error_code ec)
{
  std::cout << "Poller error: " << ec.message() << std::endl;
  return false;
}

struct Reader
{
  UserEvent& event;
  epoll::PollLoop loop;

  Reader(UserEvent& event) : event(event)
  {
    loop.set_on_initialize([this](NoticeBoard& notice_board) {
        using namespace std::placeholders;
        notice_board.add_notice(this->event.handle(),
          std::bind(std::ref(*this), _1, _2), { Notice::Inbound });
        return true;
        });
    loop.set_on_error(&print_error);
  }

  bool operator()(Notice::Event e, NoticeId id)
  {
    uint64_t value;
    event.read(value, sizeof(value));
    if (value == 42) {
      std::cout << "Writer quits, exiting reader." << std::endl;
      loop.quit();
      return true;
    }
    std::cout << "User event read: " << value << std::endl;
    return true;
  }
};

struct Writer
{
  UserEvent& event;
  uint64_t value;
  epoll::PollLoop loop;

  Writer(UserEvent& event, uint64_t value) : event(event), value(value)
  {
    loop.set_on_initialize([this](NoticeBoard& notice_board) {
        using namespace std::placeholders;
        notice_board.add_notice(this->event.handle(),
          std::bind(std::ref(*this), _1, _2), { Notice::Outbound });
        return true;
        });
    loop.set_on_error(&print_error);
  }

  bool operator()(Notice::Event e, NoticeId id)
  {
    if (value < 10) {
      std::cout << "Writing user event : " << value << std::endl;
      event.write(value++, sizeof(value));
    } else {
      event.write(42, sizeof(value));
      loop.quit();
    }
    return true;
  }
};

int main()
{
  UserEvent event(0);
  Reader reader(event);
  Writer writer(event, 1);
  std::thread t1(std::ref(reader.loop)), t2(std::ref(writer.loop));
  t1.join();
  t2.join();
  std::cout << "Event loop exited." << std::endl;
}

