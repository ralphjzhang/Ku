/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <strings.h>
#include <exception>
#include "buffer_queue.hpp"

namespace ku { namespace log {

BufferQueue::BufferQueue(uint32_t capacity) : capacity_(capacity), size_(0)
{
  if (!(nodes_ = static_cast<Node*>(::malloc(sizeof(Node) * capacity)))) {
    capacity_ = 0;
    throw std::bad_alloc();
  }
}

BufferQueue::~BufferQueue()
{
  for (uint32_t n = 0; n < size_; ++n)
    ::free(nodes_[n].data);
  ::free(nodes_);
}

void BufferQueue::reserve(uint32_t n)
{
  if (n > capacity()) {
    Node* new_nodes = static_cast<Node*>(::realloc(nodes_, sizeof(Node) * n));
    if (!new_nodes)
      throw std::bad_alloc();
    nodes_ = new_nodes;
    capacity_ = n;
    // TODO think of if this is needed
    ::bzero(nodes_ + capacity(), sizeof(Node) * (n - capacity()));
  }
}

void BufferQueue::allocate_space(size_t n)
{
  size_t nodes_size = (n - 1) / Buffer::base_size() + 1;
  reserve(nodes_size);
  for (size_t m = 0; m < nodes_size; ++m) {
    if (!(nodes_[m].data = static_cast<char*>(::malloc(sizeof(Node) * Buffer::base_size()))))
      throw std::bad_alloc();
    ++size_;
  }
}

void BufferQueue::combine(BufferQueue&& queue)
{
  // If one of us have enough capacity to hold combined data, don't realloc
  if (queue.capacity_ >= size_ + queue.size_)
    swap(queue);
  if (capacity_ >= size_ + queue.size_) {
    std::memcpy(nodes_ + size_, queue.nodes_, sizeof(Node) * queue.size_);
    size_ += queue.size_;
    queue.size_ = 0;
    return;
  }
  // Otherwise, append the shorter after the longer
  if (queue.size_ > size_)
    swap(queue);
  reserve(queue.size_ + size_);
  std::memcpy(nodes_ + size_, queue.nodes_, sizeof(Node) * queue.size_);
  size_ += queue.size_;
  queue.size_ = 0;
}

} } // namespace ku::log

