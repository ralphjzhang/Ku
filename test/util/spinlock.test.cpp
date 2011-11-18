#include <mutex>
#include <thread>

#include <utest.hpp>
#include <ku/util/spinlock.hpp>

using namespace ku::util;

TEST(spinlock, write)
{
  size_t var(0);
  const static size_t loop = 10000, con = 5;
  Spinlock spin; 

  auto write = [&]() {
    for (int i = 0; i < loop; ++i) {
      std::lock_guard<Spinlock> lock(spin);
      ++var;
    }
  };
  std::vector<std::thread> threads(con);
  for (std::thread& t : threads) {
    std::thread(write).swap(t);
  }
  for (std::thread& t : threads)
    t.join();
  EXPECT_EQ(loop * con, var);
}


