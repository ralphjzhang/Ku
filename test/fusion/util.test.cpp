#include <utest.hpp>
#include <ku/fusion/util.hpp>

using namespace ku::fusion;

TEST(Util, ceiling_pow_of_two)
{
  EXPECT_EQ(0u, util::ceiling_pow_of_two(0));
  EXPECT_EQ(1u, util::ceiling_pow_of_two(1));
  EXPECT_EQ(2u, util::ceiling_pow_of_two(2));
  EXPECT_EQ(4u, util::ceiling_pow_of_two(3));
  EXPECT_EQ(4u, util::ceiling_pow_of_two(4));
  EXPECT_EQ(8u, util::ceiling_pow_of_two(5));
  EXPECT_EQ(8u, util::ceiling_pow_of_two(6));
  EXPECT_EQ(8u, util::ceiling_pow_of_two(7));
  EXPECT_EQ(8u, util::ceiling_pow_of_two(8));
}

TEST(Util, next_pow_of_two)
{
  EXPECT_EQ(1u, util::next_pow_of_two(0));
  EXPECT_EQ(2u, util::next_pow_of_two(1));
  EXPECT_EQ(4u, util::next_pow_of_two(2));
  EXPECT_EQ(4u, util::next_pow_of_two(3));
  EXPECT_EQ(8u, util::next_pow_of_two(4));
  EXPECT_EQ(8u, util::next_pow_of_two(5));
  EXPECT_EQ(8u, util::next_pow_of_two(6));
  EXPECT_EQ(8u, util::next_pow_of_two(7));
  EXPECT_EQ(16u, util::next_pow_of_two(8));
}

