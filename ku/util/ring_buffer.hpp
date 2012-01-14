#pragma once
/***************************************************************************************************
 * A simple lock-free ring buffer that works ONLY for single producer and single consumer
 **************************************************************************************************/
#include <atomic>
#include <array>

namespace ku { namespace util {

template <typename T, unsigned int Size>
class RingBuffer
{
public:
  RingBuffer() : head_(0), tail_(0)
  { }
  ~RingBuffer() = default;

  size_t size() const
  {
    return Size;
  }

  bool empty() const
  {
    return head_ == tail_;
  }

  bool full() const
  {
    return head_ == (tail_ + 1) % capacity();
  }

  void push(T const& t)
  {
    data_[get_and_incr(tail_)] = t;
  }

  T pop()
  {
    return data_[get_and_incr(head_)];
  }

private:
  constexpr size_t capacity() const
  {
    return size() + 1;
  }

  size_t get_and_incr(std::atomic<size_t>& v) const
  {
    size_t ret = v;
    v = (v + 1) % capacity();
    return ret;
  }

private:
  std::atomic<size_t> head_, tail_;
  std::array<T, capacity()> data_;
};

} } // namespace ku::util

