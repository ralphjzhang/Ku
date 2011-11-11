#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <limits>
#include <ku/fusion/notice.hpp>
#include <ku/fusion/notice_board.hpp>
#include <ku/fusion/user_event.hpp>
#include <ku/fusion/epoll_poller.hpp>

using namespace ku::fusion;

bool print_error(std::error_code ec)
{
  std::cout << "Poller error: " << ec.message() << std::endl;
  return false;
}

void print_timing(bool reader)
{
  using namespace std::chrono;
  static time_point<system_clock> write, read;
  if (reader) {
    read = system_clock::now();
    std::cout << "Write/Read pair takes: " << duration_cast<microseconds>(read - write).count()
      << " microseconds." << std::endl;
  } else {
    write = system_clock::now();
  }
}

struct Reader
{
  UserEvent& event;
  epoll::PollLoop loop;

  Reader(UserEvent& event) : event(event)
  {
    using namespace std::placeholders;
    loop.notices().add_notice(event.handle(), std::bind(std::ref(*this), _1, _2),
        { Notice::Inbound, Notice::Edge });
    loop.set_on_error(&print_error);
  }

  bool operator()(Notice::Event e, NoticeId id)
  {
    uint64_t value;
    event.read(value);
    print_timing(true);
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
    using namespace std::placeholders;
    loop.notices().add_notice(event.handle(), std::bind(std::ref(*this), _1, _2),
        { Notice::Outbound, Notice::Edge });
    loop.set_on_error(&print_error);
  }

  bool operator()(Notice::Event e, NoticeId id)
  {
    if (value < 10) {
      std::cout << "Writing user event : " << value << std::endl;
      print_timing(false);
      event.write(value++);
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

