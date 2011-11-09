/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstdlib>
#include <cstring>
#include <utility>
#include <array>
#include <string>
#include "util.hpp"
#include "log_level.hpp"

struct iovec;

namespace ku { namespace log {

class BufferQueue;

class Buffer : private util::noncopyable
{
  friend class Collector;
  friend class Logger;
  friend class BufferQueue;
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
    NodeList() : nodes_(data_), capacity_(InitialCapacity), size_(0)
    { }

    // Recycle heap space from back of free_nodes
    NodeList(Node* free_nodes, uint32_t nodes_size)
      : nodes_(data_), capacity_(InitialCapacity), size_(std::min(nodes_size, capacity_))
    { std::memcpy(nodes_, free_nodes + nodes_size - size_, sizeof(Node) * size_); }

    NodeList(NodeList&& list)
      : nodes_(list.nodes_), capacity_(list.capacity_), size_(list.size_)
    {
      if (list.nodes_ == list.data_) {
        std::memcpy(data_, list.data_, sizeof(Node) * list.size_);
        nodes_ = data_;
      }
      list.nodes_ = nullptr;
      list.size_ = 0;
    }

    ~NodeList() { if (capacity_ > InitialCapacity) ::free(nodes_); }

    Node const* raw_data() const { return nodes_; }
    uint32_t capacity() const { return capacity_; }
    uint32_t size() const { return size_; }

    void reserve(uint32_t n);
    void emplace_back(char* data, size_t used) { nodes_[size_++] = { data, used }; }
    void append(NodeList const& list);
    void clear() { size_ = 0; }

    void swap(NodeList& list);

    Node& operator[](uint32_t n) { return nodes_[n]; }
    Node const& operator[](uint32_t n) const { return nodes_[n]; }

  private:
    const static uint32_t InitialCapacity = 2; // Keeps NodeList in 48 bytes, so Buffer object fits cache
    Node data_[InitialCapacity];
    Node* nodes_;
    uint32_t capacity_, size_; // logging buffer won't be too big, 4G nodes = 1T bytes
  };

public:
  Buffer() : size_(0) { }
  Buffer(BufferQueue& free_queue); // Try to construct a Buffer from recycled heap space
  Buffer(Buffer&& buf) : size_(buf.size_), nodes_(std::move(buf.nodes_)) { buf.size_ = 0; }
  ~Buffer();

  iovec const* raw_buffer() const { return reinterpret_cast<iovec const*>(nodes_.raw_data()); }
  uint32_t raw_buffer_count() const { return nodes_.size(); }

  void append(char const* str, size_t count);
  void append(char c);

  // swap is the common sense swap
  void swap(Buffer& buf)
  {
    nodes_.swap(buf.nodes_);
    std::swap(size_, buf.size_);
  }

  void reserve(size_t n);
  static size_t base_size() { return BaseSize; }
  size_t size() const { return size_; }
  size_t capacity() const { return nodes_.size() * BaseSize; }
  bool empty() const { return size_ == 0; }
  void clear() { nodes_.clear(); size_ = 0; }

private:
  Node& end_node() { return nodes_[size_ >> BaseBit]; }

private:
  const static size_t BaseBit = 8, BaseSize = 1 << BaseBit;
  size_t size_;
  NodeList nodes_;
};

std::string to_str(Buffer const& buf);

} } // namespace ku::log

