#include <iostream>
#include <ku/dan/dan.hpp>
#include "chuan.hpp"

TEST(chuan, chuan)
{ 
  using namespace ku::chuan;

  char buf[100];
  char* str = chuan(buf, "hello");
  *str = '\0';
  EXPECT_STREQ(buf, "hello");

  *str++ = ',';
  char const* w = "world";
  str = chuan(str, w);
  *str = '\0';
  EXPECT_STREQ(buf, "hello,world");

  *chuan(buf, 1234) = '\0';
  EXPECT_STREQ(buf, "1234");
  *chuan(buf, 12345678901234567890u) = '\0';
  EXPECT_STREQ(buf, "12345678901234567890");
}

TEST(chuan, digits)
{
  using namespace ku::chuan;

  EXPECT_EQ(digits(0xFFFFFFFFFFFFFFFFu), 20);
  EXPECT_EQ(digits(12345678901234567890u), 20);
  EXPECT_EQ(digits(10000000000000000000u), 20);
  EXPECT_EQ(digits(1234567890123456789u), 19);
  EXPECT_EQ(digits(1000000000000000000u), 19);
  EXPECT_EQ(digits(123456789012345678u), 18);
  EXPECT_EQ(digits(100000000000000000u), 18);
  EXPECT_EQ(digits(12345678901234567u), 17);
  EXPECT_EQ(digits(10000000000000000u), 17);
  EXPECT_EQ(digits(1234567890123456u), 16);
  EXPECT_EQ(digits(1000000000000000u), 16);
  EXPECT_EQ(digits(123456789012345u), 15);
  EXPECT_EQ(digits(100000000000000u), 15);
  EXPECT_EQ(digits(12345678901234u), 14);
  EXPECT_EQ(digits(10000000000000u), 14);
  EXPECT_EQ(digits(1234567890123u), 13);
  EXPECT_EQ(digits(1000000000000u), 13);
  EXPECT_EQ(digits(123456789012u), 12);
  EXPECT_EQ(digits(100000000000u), 12);
  EXPECT_EQ(digits(12345678901u), 11);
  EXPECT_EQ(digits(10000000000u), 11);
  EXPECT_EQ(digits(9234567890u), 10);
  EXPECT_EQ(digits(0xFFFFFFFFu), 10);

  EXPECT_EQ(digits(1234567890u), 10);
  EXPECT_EQ(digits(1000000000u), 10);
  EXPECT_EQ(digits(123456789u), 9);
  EXPECT_EQ(digits(100000000u), 9);
  EXPECT_EQ(digits(12345678u), 8);
  EXPECT_EQ(digits(10000000u), 8);
  EXPECT_EQ(digits(1234567u), 7);
  EXPECT_EQ(digits(1000000u), 7);
  EXPECT_EQ(digits(123456u), 6);
  EXPECT_EQ(digits(100000u), 6);
  EXPECT_EQ(digits(12345u), 5);
  EXPECT_EQ(digits(10000u), 5);
  EXPECT_EQ(digits(1234u), 4);
  EXPECT_EQ(digits(1000u), 4);
  EXPECT_EQ(digits(123u), 3);
  EXPECT_EQ(digits(100u), 3);
  EXPECT_EQ(digits(12u), 2);
  EXPECT_EQ(digits(10u), 2);
  EXPECT_EQ(digits(1u), 1);
  EXPECT_EQ(digits(0u), 1);
}

