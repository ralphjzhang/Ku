/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <sys/uio.h>
#include <cstring>
#include <cassert>
#include <exception>
#include <algorithm>
#include "buffer.hpp"
#include "buffer_list.hpp"

#include <iostream>

namespace ku { namespace log {

/// Buffer::NodeList ///
//
void Buffer::NodeList::reserve(uint32_t n)
{
  if (n > capacity()) {
    if (nodes_ == data_) {
      // Nodes are on the stack, move them to heap
      Node* new_nodes = static_cast<Node*>(::malloc(sizeof(Node) * n));
      if (!new_nodes)
        throw std::bad_alloc();
      nodes_ = new_nodes;
      std::memcpy(nodes_, data_, sizeof(data_));
      capacity_ = n;
    } else {
      // Nodes are already on the heap, expand the heap space
      Node* new_nodes = static_cast<Node*>(::realloc(nodes_, sizeof(Node) * n));
      if (!new_nodes)
        throw std::bad_alloc();
      nodes_ = new_nodes;
      capacity_ = n;
    }
  }
}

void Buffer::NodeList::append(NodeList const& list)
{
  reserve(size() + list.size());
  std::memcpy(nodes_ + size(), list.nodes_, sizeof(Node) * list.size());
  size_ += list.size_;
}

void Buffer::NodeList::swap(NodeList& list)
{
  if (capacity() > InitialCapacity) {
    if (list.capacity() > InitialCapacity) {
      // Both have nodes on the heap
      std::swap(nodes_, list.nodes_);
    } else {
      // We are on heap, list is on stack
      list.nodes_ = nodes_;
      std::memcpy(data_, list.data_, sizeof(data_));
      nodes_ = data_;
    }
  } else {
    if (list.capacity() > InitialCapacity) {
      // We are on stack, list is on heap
      nodes_ = list.nodes_;
      std::memcpy(list.data_, data_, sizeof(data_));
      list.nodes_ = list.data_;
    } else {
      // Both on stack
      char temp[sizeof(data_)];
      std::memcpy(temp, data_, sizeof(data_));
      std::memcpy(data_, list.data_, sizeof(data_));
      std::memcpy(list.data_, temp, sizeof(data_));
    }
  }
  std::swap(capacity_, list.capacity_);
  std::swap(size_, list.size_);
}


/// Buffer ///
//
Buffer::~Buffer()
{
  for (size_t n = 0; n < nodes_.size(); ++n)
    ::free(nodes_[n].data);
}

void Buffer::append(char const* str, size_t count)
{
  reserve(size() + count);
  assert(count <= capacity() - size());
  while (count) {
    Node& node = end_node();
    char *end = node.data + node.used;
    size_t cp_size = std::min(count, BaseSize - node.used);
    std::memcpy(end, str, cp_size);
    node.used += cp_size;
    str += cp_size;
    size_ += cp_size;
    count -= cp_size;
  }
}

void Buffer::append(char c)
{
  reserve(size() + 1);
  assert(1 <= capacity() - size());
  Node& node = end_node();
  node.data[node.used++] = c;
  ++size_;
}

void Buffer::reserve(size_t n)
{
  if (n > capacity()) {
    size_t over_size = n - capacity();
    size_t div = over_size >> BaseBit;
    size_t new_nodes = div + (over_size - (div << BaseBit) != 0);
    nodes_.reserve(nodes_.size() + new_nodes);
    for (size_t i = 0; i < new_nodes; ++i) {
      void* p = ::malloc(BaseSize);
      if (!p) throw std::bad_alloc();
      nodes_.emplace_back(static_cast<char*>(p), 0);
    }
  }
}

Buffer::Buffer(BufferList& free_queue) : size_(0), nodes_(free_queue.nodes_, free_queue.size_)
{
  free_queue.size_ -= nodes_.size();
}

std::string to_str(Buffer const& buf)
{
  // This one is mostly for debugging, no need to optimize much
  std::string str;
  size_t count = buf.size();
  str.reserve(count + 1);
  while (count) {
    Buffer::Node const& node = buf.nodes_[(buf.size() - count) >> Buffer::BaseBit];
    str.append(node.data, node.used);
    assert(count >= node.used);
    count -= node.used;
  }
  return str;
}

} } // namespace ku::log

