/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstddef>
#include <type_traits>
#include <initializer_list>
#include <atomic>
#include <vector>

namespace ku { namespace fusion {

class Sequence
{
public:
  Sequence(size_t value) { set(value); }
  ~Sequence() = default;

  // These get/set work in the assumption that there is only one writer
  size_t get() const { return value_.load(std::memory_order_acquire); }
  void set(size_t value) { value_.store(value, std::memory_order_release); }
  bool cas(size_t old_value, size_t new_value) { return value_.compare_exchange_strong(old_value, new_value); }

private:
  union {
    std::aligned_storage<64, 64>::type _; // align to/ocuppy 64 bytes for cache line
    std::atomic_size_t value_;
  };
};

static_assert(std::alignment_of<Sequence>::value == 64, "Sequence not aligned with cache line, may cause false sharing.");
static_assert(sizeof(Sequence) == 64, "Sequence not padded to cache line, may cause false sharing.");


class SequenceList
{
public:
  size_t min_sequence() const;
  void initialize(std::initializer_list<Sequence*> const& seqs);

private:
  std::vector<Sequence*> list_;
};

} } // namespace ku::fusion

