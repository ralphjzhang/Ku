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
  : event_buffer_(buf_size), active_count_(0), quit_(false)
{
  if ((raw_handle_ = epoll_create1(flags)) == -1)
    throw std::system_error(util::errc(), "Poller::Poller");
}

void Poller::loop(std::chrono::milliseconds const& timeout)
{
  while (!quit_)
    poll(timeout);
}

void Poller::operator()(std::chrono::milliseconds const& timeout)
{
  loop(timeout);
}

bool Poller::add_event(int raw_fd, epoll_event* event)
{
  return ::epoll_ctl(raw_handle_, EPOLL_CTL_ADD, raw_fd, event) == 0;
}

bool Poller::remove_event(int raw_fd)
{
  return ::epoll_ctl(raw_handle_, EPOLL_CTL_DEL, raw_fd, nullptr) == 0 || errno == EBADF;
}

bool Poller::modify_event(int raw_fd, epoll_event* event)
{
  return ::epoll_ctl(raw_handle_, EPOLL_CTL_MOD, raw_fd, event) == 0;
}

void Poller::poll(std::chrono::milliseconds const& timeout)
{
  size_t begin_seq = event_buffer_.published_sequence() + 1;
  epoll_event* free_event_begin = &event_buffer_.raw_event(begin_seq);
  size_t next_seq;
  while ((next_seq = event_buffer_.max_usable_block_sequence()) < begin_seq)
    std::this_thread::yield(); // TODO wait strategy

  event_buffer_.claim(next_seq);
  int event_num = ::epoll_wait(raw_handle_, free_event_begin, next_seq - begin_seq + 1,
                               timeout.count());
  if (event_num > 0) {
    event_buffer_.publish(begin_seq + event_num - 1);
  } else if (event_num == -1) { // TODO EINTR
    throw std::system_error(util::errc(), "Poller::poll");
  }
}

} } // namespace ku::fusion

