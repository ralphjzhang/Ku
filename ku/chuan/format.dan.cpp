#include <iostream>
#include <functional>
#include <ku/dan/dan.hpp>
#include "format.hpp"

TEST(chuan, format)
{
  using namespace ku::chuan;
  using namespace std::placeholders;

  char str[256];
  auto fmt = make_lit("Life, ") |_1| " and " |_2;
  *format(str, fmt, "universe", "everything") = '\0';
  EXPECT_STREQ(str, "Life, universe and everything");

  //format(buf, "chuan::format arg 1:"|_1|", arg2:"|_2|", better?", 42, "life");
}

