#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ku/reflex/func_traits.hpp>
#include "stopwatch.hpp"

namespace ku { namespace util {

class Perf
{
public:
  Perf(size_t loop) : loop_(loop) { }

  template <typename F, typename... Args>
  void operator()(F f, Args&&... args) { return run(f, args...); }

  template <typename F, typename... Args>
  void run()(F f, Args&&... args)
  {
    Stopwatch sw;
    sw.start();
    for (unsigned i = 0; i < loop_; ++i)
      f(std::forward<Args>(args)...);
    sw.stop();
    print(ku::reflex::func_traits(f).fullname(), sw.elapsed_nanoseconds());
  }

  void print(std::string const& name, uint64_t ns)
  {
    static const uint64_t E9 = 1000000000;
    std::lldiv_t tm = std::div(ns, E9);
    uint64_t avg = ns / loop_;
    std::cout << std::setw(20) << std::setfill(' ') << type_name<T>()
      << " takes: " << std::setw(3) << std::setfill(' ') << tm.quot << "." 
      << std::setw(9) << std::setfill('0') << tm.rem << '\t'  
      << "Average: " << avg << " nanosec" << std::endl;
  }

private:
  size_t loop_;
};

} } // namespace ku::util

