/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <system_error>
#include <thread>
#include "poller.hpp"
#include "util.hpp"

namespace ku { namespace fusion {

Poller::Poller(int flags, size_t buf_size)
  : event_buffer_(buf_size), active_count_(0)
{
  if ((raw_handle_ = epoll_create1(flags)) == -1)
    throw std::system_error(util::errc(), "Poller::Poller");
}

void Poller::poll(std::chrono::milliseconds const& timeout)
{
  size_t begin_seq = event_buffer_.published_sequence() + 1;
  epoll_event* free_event_begin = &event_buffer_.raw_event(begin_seq);
  size_t next_seq;
  while ((next_seq = event_buffer_.max_usable_block_sequence()) <= begin_seq)
    std::this_thread::yield(); // TODO wait strategy

  event_buffer_.claim(next_seq);
  int event_num = ::epoll_wait(raw_handle_, free_event_begin, next_seq - begin_seq + 1,
                               timeout.count());
  if (event_num == -1) // TODO EINTR
    throw std::system_error(util::errc(), "Poller::poll");
  if (event_num > 0)
    event_buffer_.publish(begin_seq + event_num - 1);
}

} } // namespace ku::fusion

