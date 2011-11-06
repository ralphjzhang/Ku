/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <strings.h>
#include <cstdlib>
#include <utility>
#include <array>
#include <string>
#include "util.hpp"
#include "log_level.hpp"

struct iovec;

namespace ku { namespace log {

class Buffer : private util::noncopyable
{
  friend class Collector;
  friend class Logger;
  friend std::string to_str(Buffer const& buf);

private:
  // This maps iovec, can be casted directly to iovec
  struct Node
  {
    char* operator[](size_t n) { return data + n; }

    char* data;
    size_t used;
  };

  class NodeList : private util::noncopyable
  {
  public:
    NodeList() : nodes_(&data_[0]), capacity_(InitialCapacity), size_(0)
    { ::bzero(&data_, sizeof(data_)); }

    NodeList(NodeList& list);  // This is NOT copy ctor, it tries to recycle nodes from list
    ~NodeList() { if (capacity_ > InitialCapacity) ::free(nodes_); }

    Node const* raw_data() const { return nodes_; }
    size_t capacity() const { return capacity_; }
    size_t size() const { return size_; }

    void reserve(size_t n);
    void emplace_back(char* data, size_t used) { nodes_[size_++] = { data, used }; }
    void append(NodeList const& list);
    void clear() { ::bzero(&nodes_, sizeof(Node) * size()); size_ = 0; }

    void swap(NodeList& list);

    Node& operator[](size_t n) { return nodes_[n]; }
    Node const& operator[](size_t n) const { return nodes_[n]; }

  private:
    const static uint32_t InitialCapacity = 3; // Keeps NodeList in 64 bytes, fits cache line
    Node* nodes_;
    uint32_t capacity_, size_; // logging buffer won't be too big, 4G nodes = 512G bytes
    std::array<Node, InitialCapacity> data_;
  };

public:
  Buffer() : size_(0) { }
  ~Buffer();

  iovec const* raw_buffer() const { return reinterpret_cast<iovec const*>(nodes_.raw_data()); }
  size_t raw_buffer_count() const { return nodes_.size(); }

  void append(char const* str, size_t count);
  void append(char c);

  // swap is the common sense swap
  void swap(Buffer& buf)
  {
    nodes_.swap(buf.nodes_);
    std::swap(size_, buf.size_);
  }

  // exchange is like submit data, and get some free space for exchange. TODO
  size_t exchange(Buffer& buf);
  void reserve(size_t n);

  size_t base_size() const { return BaseSize; }
  size_t size() const { return size_; }
  size_t capacity() const { return nodes_.size() * BaseSize; }
  bool empty() const { return size_ == 0; }

  LogLevel log_level() { return log_level_; }
  void set_log_level(LogLevel log_level) { log_level_ = log_level; }

private:
  // This is NOT copy ctor, it tries to recycle space from parameter
  Buffer(Buffer& buf) : size_(0), nodes_(buf.nodes_) { }

  // Warning: after combine_buffer, the storage might not be continous, this function is 
  // supposed to be the last operation before calling ::writev
  void combine_buffer(Buffer& buf);
  void clear() { nodes_.clear(); size_ = 0; }

  Node& end_node() { return nodes_[size_ >> BaseBit]; }
  // Drop data, but recycle the space that is already allocated
  void reclaim();

private:
  const static size_t BaseBit = 7, BaseSize = 1 << BaseBit;
  size_t size_;
  NodeList nodes_;
  LogLevel log_level_;
};

std::string to_str(Buffer const& buf);

} } // namespace ku::log

