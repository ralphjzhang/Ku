#include <iostream>
#include <vector>
#include <forward_list>
#include <memory>
#include <thread>
#include <numeric>

#include <ku/fusion/notice.hpp>
#include <ku/fusion/notice_board.hpp>
#include <ku/fusion/user_event.hpp>
#include <ku/fusion/epoll_poller.hpp>

using namespace ku::fusion;

int g_data = 0;

struct Writer
{
  WriterUserEvent event;
  epoll::PollLoop loop;
  int value;

  Writer()
  {
    value = 10;
    loop.notices().add_notice(event.handle(), { Notice::Outbound, Notice::Edge },
        [this](Notice::Event, NoticeId) { write(); return true; });
  }

  void write()
  {
    g_data = value--;
    event.write();
    if (value <= 0)
      loop.quit();
  }
};

struct Reader
{
  ReaderUserEvent event;
  epoll::PollLoop loop;
  std::vector<int> data;

  Reader(WriterUserEvent& writer) : event(writer)
  {
    loop.notices().add_notice(event.handle(), { Notice::Inbound, Notice::Edge },
        [this](Notice::Event, NoticeId) { read(); return true; });
  }

  void read()
  {
    // Read data
    data.push_back(g_data);
    for (int i : data) std::cout << i << " ";
    std::cout << std::endl;
    event.read();
  }
};

int main()
{
  Writer writer;
  Reader reader(writer.event);
  std::thread writer_t(std::ref(writer.loop));
  std::thread reader_t(std::ref(reader.loop));

  writer.event.write();
  writer_t.join();
  reader.loop.quit();
  reader_t.join();
}

