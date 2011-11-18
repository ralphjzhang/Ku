#include <cassert>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <ku/util/spinlock.hpp>
#include <ku/util/perf.hpp>

size_t atomics(size_t loop, size_t n)
{
  std::atomic_size_t data(0);
  auto add = [&]() {
    for (size_t i = 0; i < loop; ++i)
      ++data;
  };
  std::vector<std::thread> threads(n);
  for (auto& t : threads)
    std::thread(add).swap(t);
  for (auto& t : threads)
    t.join();
  assert(data == loop * n);
  return data;
}

size_t spinlocks(size_t loop, size_t n)
{
  size_t data(0);
  ku::util::Spinlock lock;
  auto add = [&]() {
    for (size_t i = 0; i < loop; ++i) {
      std::lock_guard<ku::util::Spinlock> l(lock);
      ++data;
    }
  };
  std::vector<std::thread> threads(n);
  for (auto& t : threads)
    std::thread(add).swap(t);
  for (auto& t : threads)
    t.join();
  assert(data == loop * n);
  return data;
}

size_t mutexes(size_t loop, size_t n)
{
  size_t data(0);
  std::mutex mutex;
  auto add = [&]() {
    for (size_t i = 0; i < loop; ++i) {
      std::lock_guard<std::mutex> l(mutex);
      ++data;
    }
  };
  std::vector<std::thread> threads(n);
  for (auto& t : threads)
    std::thread(add).swap(t);
  for (auto& t : threads)
    t.join();
  assert(data == loop * n);
  return data;
}

int main()
{
  ku::util::Perf run(1);
  static const size_t loop = 10000, n = 10;
  atomics(loop, n);
  run(atomics, loop, n);
  run(spinlocks, loop, n);
  run(mutexes, loop, n);
}

