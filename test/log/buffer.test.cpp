#include <utest.hpp>
#include <string>
#include <ku/log/buffer.hpp>

using namespace ku::log;

TEST(Buffer, basic)
{
  Buffer buf;
  EXPECT_EQ(0u, buf.size());
  EXPECT_EQ(0u, buf.capacity());
  buf.set_log_level(LogLevel::Fatal);
  EXPECT_EQ(LogLevel::Fatal, buf.log_level());
}

TEST(Buffer, reserve)
{
  Buffer buf;
  buf.reserve(1);
  EXPECT_EQ(buf.base_size(), buf.capacity());
  buf.reserve(buf.base_size());
  EXPECT_EQ(buf.base_size(), buf.capacity());
  buf.reserve(buf.base_size() + 1);
  EXPECT_EQ(2 * buf.base_size(), buf.capacity());
  buf.reserve(buf.base_size() - 1);
  EXPECT_EQ(2 * buf.base_size(), buf.capacity()); // doesn't shrink

  buf.reserve(buf.base_size() * 5 + 1);
  EXPECT_EQ(6 * buf.base_size(), buf.capacity());

  EXPECT_EQ(0u, buf.size());
}

TEST(Buffer, append)
{
  Buffer buf;
  buf.reserve(buf.base_size());
  std::string s('a', buf.base_size() / 2);
  buf.append(s.c_str(), s.size());
  EXPECT_EQ(s.size(), buf.size());
}


