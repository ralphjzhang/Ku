/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstdlib>
#include <utility>
#include "log_level.hpp"

struct iovec;

namespace ku { namespace log {

class Buffer
{
private:
  // This maps iovec, can be casted directly to iovec
  struct Node
  {
    char* operator[](size_t n) { return data + n; }

    char* data;
    size_t used;
  };

  class NodeList
  {
  public:
    NodeList();
    ~NodeList();

    Node const* raw_data() const { return nodes_; }
    size_t capacity() const { return capacity_; }
    size_t size() const { return size_; }

    void reserve(size_t n);
    void emplace_back(char* data, size_t used) { nodes_[size_++] = { data, used }; }

    void swap(NodeList& list)
    {
      std::swap(capacity_, list.capacity_);
      std::swap(size_, list.size_);
      std::swap(nodes_, list.nodes_);
    }

    Node& operator[](size_t n) { return nodes_[n]; }

  private:
    const static size_t InitialCapacity = 4;
    size_t capacity_, size_;
    Node* nodes_;
  };

public:
  Buffer() : base_(7), size_(0) { }

  iovec const* raw_buffer() const { return reinterpret_cast<iovec const*>(nodes_.raw_data()); }
  size_t raw_buffer_count() const { return nodes_.size(); }

  void append(char const* str, size_t count);

  // swap is the common sense swap
  void swap(Buffer& buf)
  {
    std::swap(nodes_, buf.nodes_);
    std::swap(size_, buf.size_);
  }

  // exchange is like submit data, and get some free space for exchange.
  size_t exchange(Buffer& buf);
  void reserve(size_t n);

  size_t base_size() const { return 1 << base_; }
  size_t size() const { return size_; }
  size_t capacity() const { return nodes_.size() * base_size(); }

  LogLevel log_level() { return log_level_; }
  void set_log_level(LogLevel log_level) { log_level_ = log_level; }

private:
  Node& end_node() { return nodes_[size_ >> base_]; }
  // Drop data, but recycle the space that is already allocated
  void reclaim();

private:
  const size_t base_;
  size_t size_;
  NodeList nodes_;
  LogLevel log_level_;
};

} } // namespace ku::log

