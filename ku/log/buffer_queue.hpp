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

class BufferQueue : private util::noncopyable
{
  typedef Buffer::Node Node;
  friend Buffer::Buffer(BufferQueue&);

public:
  BufferQueue() : capacity_(0), size_(0), nodes_(nullptr) { }
  BufferQueue(uint32_t capacity);
  BufferQueue(BufferQueue&& queue)
    : capacity_(queue.capacity_), size_(queue.size_), nodes_(queue.nodes_)
  { queue.capacity_ = 0; queue.size_ = 0; queue.nodes_ = nullptr; }
  ~BufferQueue();

  iovec const* raw_buffer() const { return reinterpret_cast<iovec const*>(nodes_); }
  uint32_t raw_buffer_count() const { return size_; }

  void swap(BufferQueue& queue)
  {
    std::swap(capacity_, queue.capacity_);
    std::swap(size_, queue.size_);
    std::swap(nodes_, queue.nodes_);
  }

  void reserve(uint32_t n); // reserve n nodes
  void allocate_space(size_t n); // allocate at least n bytes free space

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

  // Combine nodes to recycle dynamic space
  void combine(BufferQueue&& queue);

  uint32_t capacity() const { return capacity_; }
  uint32_t size() const { return size_; }

private:
  uint32_t capacity_, size_;
  Node* nodes_;
};

} } // namespace ku::log

