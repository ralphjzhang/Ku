/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <exception>
#include "buffer_list.hpp"

namespace ku { namespace log {

BufferList::BufferList(uint32_t capacity) : capacity_(capacity), size_(0)
{
  nodes_ = static_cast<Node*>(::malloc(sizeof(Node) * capacity));
  if (!nodes_) {
    capacity_ = 0;
    throw std::bad_alloc();
  }
}

BufferList::~BufferList()
{
  for (uint32_t n = 0; n < size_; ++n)
    ::free(nodes_[n].data);
  ::free(nodes_);
}

void BufferList::reserve(uint32_t n)
{
  if (n > capacity_) {
    Node* new_nodes = nodes_ ? static_cast<Node*>(::realloc(nodes_, sizeof(Node) * n))
                             : static_cast<Node*>(::malloc(sizeof(Node) * n));
    if (!new_nodes)
      throw std::bad_alloc();
    nodes_ = new_nodes;
    capacity_ = n;
  }
}

void BufferList::allocate_space(size_t n)
{
  size_t nodes_size = (n - 1) / Buffer::base_size() + 1;
  reserve(nodes_size);
  for (size_t m = 0; m < nodes_size; ++m) {
    if (!(nodes_[m].data = static_cast<char*>(::malloc(sizeof(Node) * Buffer::base_size()))))
      throw std::bad_alloc();
    nodes_[m].used = 0;
    ++size_;
  }
}

void BufferList::reclaim_space()
{
  for (size_t n = 0; n < size_; ++n)
    nodes_[n].used = 0;
}

void BufferList::combine(BufferList&& list)
{
  // Append the shorter after the longer
  if (list.capacity_ >= size_ + list.size_ || list.size_ > size_)
    swap(list);
  push_back(list.nodes_, list.size_);
  list.size_ = 0;
}

} } // namespace ku::log

