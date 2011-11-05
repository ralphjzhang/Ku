/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <sys/uio.h>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "buffer.hpp"

#include <iostream>

namespace ku { namespace log {

/// Buffer::NodeList ///
//
void Buffer::NodeList::reserve(size_t n)
{
  if (n > capacity()) {
    if (nodes_ == &data_[0]) {
      // Nodes are on the stack, move them to heap
      nodes_ = static_cast<Node*>(::malloc(sizeof(Node) * n));
      ::bzero(nodes_, sizeof(Node) * n);
      std::memcpy(nodes_, &data_, sizeof(data_));
      capacity_ = n;
    } else {
      // Nodes are already on the heap, expand the heap space
      nodes_ = static_cast<Node*>(::realloc(nodes_, sizeof(Node) * n));
      assert(nodes_); // If memory is up, we are in trouble anyway...
      ::bzero(nodes_ + capacity(), sizeof(Node) * (n - capacity()));
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
      data_ = list.data_;
      nodes_ = &data_[0];
    }
  } else {
    if (list.capacity() > InitialCapacity) {
      // We are on stack, list is on heap
      nodes_ = list.nodes_;
      list.data_ = data_;
      list.nodes_ = &list.data_[0];
    } else {
      // Both on stack
      char temp[sizeof(data_)];
      std::memcpy(temp, &data_, sizeof(data_));
      std::memcpy(&data_, &list.data_, sizeof(data_));
      std::memcpy(&list.data_, temp, sizeof(data_));
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
    for (size_t i = 0; i < new_nodes; ++i)
      nodes_.emplace_back(static_cast<char*>(::malloc(BaseSize)), 0);
  }
}

void Buffer::reclaim()
{
  for (size_t n = 0; n < nodes_.size(); ++n)
    nodes_[n].used = 0;
  size_ = 0;
}

void Buffer::combine_buffer(Buffer& buf)
{
  nodes_.append(buf.nodes_);
  size_ += buf.size_;
  buf.clear();
}

std::string to_str(Buffer const& buf)
{
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

