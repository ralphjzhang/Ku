/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#include <sys/uio.h>
#include <strings.h>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "buffer.hpp"

namespace ku { namespace log {

/// Buffer::NodeList ///
//
Buffer::NodeList::NodeList()
  : capacity_(InitialCapacity), size_(0)
  , nodes_(static_cast<Node*>(::malloc(sizeof(Node) * capacity())))
{
  ::bzero(nodes_, sizeof(Node) * capacity());
}

Buffer::NodeList::~NodeList()
{
  for (size_t i = 0; i < size_; ++i)
    ::free(nodes_[i].data);
  ::free(nodes_);
}

void Buffer::NodeList::reserve(size_t n)
{
  if (n > capacity()) {
    nodes_ = static_cast<Node*>(::realloc(nodes_, sizeof(Node) * n));
    assert(nodes_); // If memory is up, we are in trouble anyway...
    ::bzero(nodes_ + capacity(), sizeof(Node) * (n - capacity()));
  }
}

/// Buffer ///
//
void Buffer::append(char const* str, size_t count)
{
  reserve(size() + count);
  assert(count <= capacity() - size());
  while (count) {
    Node& end_nd = end_node();
    char *end = end_nd.data + end_nd.used;
    size_t cp_size = std::max(count, base_size() - end_nd.used);
    memcpy(end, str, cp_size);
    end_nd.used += cp_size;
    size_ += cp_size;
    count -= cp_size;
  }
}

void Buffer::reserve(size_t n)
{
  if (n > capacity()) {
    size_t over_size = n - capacity();
    size_t div = over_size >> base_;
    size_t new_nodes = div + (over_size - div == 0);
    nodes_.reserve(nodes_.size() + new_nodes);
    for (size_t i = 0; i < new_nodes; ++i)
      nodes_.emplace_back(new char[base_size()], 0);
  }
}

void Buffer::reclaim()
{
  for (size_t i = 0; i < size(); ++i)
    nodes_[i].used = 0;
  size_ = 0;
}

} } // namespace ku::log

