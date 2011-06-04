#include <iostream>
#include <functional>
#include <ku/dan/dan.hpp>
#include "format.hpp"

TEST(chuan, format)
{
  using namespace ku::chuan;
  using namespace std::placeholders;

  char buf[256];
  //format(buf, "chuan::format arg 1:"|_1|", arg2:"|_2|", better?", 42, "life");

  auto s = format("Life, ") |_1| " and " |_2;
  print(buf, s, "universe", "everything");

  std::cout << buf << std::endl;
}

