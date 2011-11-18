#include <time.h>

namespace ku { namespace util {

class Stopwatch
{
  static const uint64_t E9 = 1000000000;

public:
  Stopwatch() : ts0({0, 0}), ts1({0, 0}) { }
  ~Stopwatch() = default;

  inline void start() { ::clock_gettime(CLOCK_REALTIME, &ts0); }
  inline void stop() { ::clock_gettime(CLOCK_REALTIME, &ts1); }

  inline uint64_t elapsed_nanoseconds() const
  { return (ts1.tv_sec - ts0.tv_sec) * E9 + ts1.tv_nsec - ts0.tv_nsec; }

private:
  timespec ts0, ts1;
};

} } // namespace ku::util

