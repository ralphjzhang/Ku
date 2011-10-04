#include <ku/dan/dan.hpp>
#include "collector.hpp"

using namespace ku::log;

TEST(Collector, append)
{
  Collector c;
  std::string s("This is a log");
  c << s << ", and the data: " << 32;

  c << "the answer is %d, but %u is also fine, %s", 42, 81, "Ralph";
}

