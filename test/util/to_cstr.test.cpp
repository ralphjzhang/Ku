#include <utest.hpp>
#include <ku/util/to_cstr.hpp>

TEST(to_cstr, to_cstr)
{ 
  using namespace ku::util;

  char buf[100];
  *to_cstr(buf, 1234) = '\0';
  EXPECT_STREQ(buf, "1234");
  *to_cstr(buf, 12345678901234567890u) = '\0';
  EXPECT_STREQ(buf, "12345678901234567890");
}

TEST(to_cstr, digits)
{
  using namespace ku::util;

  EXPECT_EQ(20, digits(0xFFFFFFFFFFFFFFFFu));
  EXPECT_EQ(20, digits(12345678901234567890u));
  EXPECT_EQ(20, digits(10000000000000000000u));
  EXPECT_EQ(19, digits(1234567890123456789u));
  EXPECT_EQ(19, digits(1000000000000000000u));
  EXPECT_EQ(18, digits(123456789012345678u));
  EXPECT_EQ(18, digits(100000000000000000u));
  EXPECT_EQ(17, digits(12345678901234567u));
  EXPECT_EQ(17, digits(10000000000000000u));
  EXPECT_EQ(16, digits(1234567890123456u));
  EXPECT_EQ(16, digits(1000000000000000u));
  EXPECT_EQ(15, digits(123456789012345u));
  EXPECT_EQ(15, digits(100000000000000u));
  EXPECT_EQ(14, digits(12345678901234u));
  EXPECT_EQ(14, digits(10000000000000u));
  EXPECT_EQ(13, digits(1234567890123u));
  EXPECT_EQ(13, digits(1000000000000u));
  EXPECT_EQ(12, digits(123456789012u));
  EXPECT_EQ(12, digits(100000000000u));
  EXPECT_EQ(11, digits(12345678901u));
  EXPECT_EQ(11, digits(10000000000u));
  EXPECT_EQ(10, digits(9234567890u));
  EXPECT_EQ(10, digits(0xFFFFFFFFu));

  EXPECT_EQ(10, digits(1234567890u));
  EXPECT_EQ(10, digits(1000000000u));
  EXPECT_EQ(9, digits(123456789u));
  EXPECT_EQ(9, digits(100000000u));
  EXPECT_EQ(8, digits(12345678u));
  EXPECT_EQ(8, digits(10000000u));
  EXPECT_EQ(7, digits(1234567u));
  EXPECT_EQ(7, digits(1000000u));
  EXPECT_EQ(6, digits(123456u));
  EXPECT_EQ(6, digits(100000u));
  EXPECT_EQ(5, digits(12345u));
  EXPECT_EQ(5, digits(10000u));
  EXPECT_EQ(4, digits(1234u));
  EXPECT_EQ(4, digits(1000u));
  EXPECT_EQ(3, digits(123u));
  EXPECT_EQ(3, digits(100u));
  EXPECT_EQ(2, digits(12u));
  EXPECT_EQ(2, digits(10u));
  EXPECT_EQ(1, digits(1u));
  EXPECT_EQ(1, digits(0u));
}

