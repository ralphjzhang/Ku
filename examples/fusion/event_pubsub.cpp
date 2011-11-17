#include <iostream>
#include <vector>
#include <forward_list>
#include <memory>
#include <thread>

#include <ku/fusion/notice.hpp>
#include <ku/fusion/notice_board.hpp>
#include <ku/fusion/user_event.hpp>
#include <ku/fusion/epoll_poller.hpp>

using namespace ku::fusion;

struct Publisher
{
  PublisherUserEvent event;
  epoll::PollLoop loop;

  Publisher()
  {
    loop.notices().add_notice(event.handle(), { Notice::Outbound, Notice::Edge },
        [this](Notice::Event, NoticeId) { publish(); return true; });
  }

  void publish()
  {
    std::cout << "Publisher notified." << std::endl;
    // Prepare data
    //if (event.try_publish())
    //  std::cout << "Publish succeeded." << std::endl;
    //sleep(3);
  }
};

struct Subscriber
{
  int id;
  SubscriberUserEvent event;
  epoll::PollLoop loop;

  Subscriber(int id, PublisherUserEvent& pub) : id(id), event(pub)
  {
    loop.notices().add_notice(event.handle(), { Notice::Inbound, Notice::Edge },
        [this](Notice::Event, NoticeId) { read(); return true; });
  }

  void read()
  {
    std::cout << "Subscriber " << id << " notified." << std::endl;
    // Read data
    event.try_read();
  }
};

int main()
{
  Publisher pub;
  std::thread pub_t(std::ref(pub.loop));
  std::vector<std::thread> sub_threads(10);
  std::forward_list<std::unique_ptr<Subscriber>> subs;
  for (int i = 0; i < sub_threads.size(); ++i) {
    subs.emplace_front(new Subscriber(i, pub.event));
    std::thread(std::ref(subs.front()->loop)).swap(sub_threads[i]);
  }

  pub.event.initialize();
  // do something

  pub_t.join();
  for (std::thread& t : sub_threads)
    t.join();
}

