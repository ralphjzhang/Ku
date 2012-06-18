/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstring>
#include <utility>
#include "util.hpp"
#include "buffer.hpp"

namespace ku { namespace log {

class BufferList : private util::noncopyable
{
  using Node = Buffer::Node;
  friend Buffer::Buffer(BufferList&);

public:
  BufferList() : capacity_(0), size_(0), nodes_(nullptr) { }
  BufferList(uint32_t capacity);
  BufferList(BufferList&& list)
    : capacity_(list.capacity_), size_(list.size_), nodes_(list.nodes_)
  { list.capacity_ = 0; list.size_ = 0; list.nodes_ = nullptr; }
  ~BufferList();

  iovec const* raw_buffer() const { return reinterpret_cast<iovec const*>(nodes_); }
  uint32_t raw_buffer_count() const { return size_; }

  void swap(BufferList& list)
  {
    std::swap(capacity_, list.capacity_);
    std::swap(size_, list.size_);
    std::swap(nodes_, list.nodes_);
  }

  void reserve(uint32_t n); // reserve n nodes
  void allocate_space(size_t n); // allocate at least n bytes free space
  void reclaim_space(); // mark all dynamic space as unused
  void shrink_space(size_t n); // shrink free space to no more than n bytes

  void push_back(Node const* node, uint32_t count)
  {
    reserve(size_ + count);
    std::memcpy(nodes_ + size_, node, sizeof(Node) * count);
    size_ += count;
  }

  void emplace_back(Buffer&& buf)
  {
    push_back(reinterpret_cast<Node const*>(buf.raw_buffer()), buf.raw_buffer_count());
    buf.clear();
  }

  void combine(BufferList&& list);

  uint32_t capacity() const { return capacity_; }
  uint32_t size() const { return size_; }

private:
  uint32_t capacity_, size_;
  Node* nodes_;
};

} } // namespace ku::log

