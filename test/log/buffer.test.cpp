#include <utest.hpp>
#include <string>
#define private public
#include <ku/log/buffer.hpp>
#undef private

using namespace ku::log;

/// Buffer::NodeList
//
TEST(NodeList, reserve)
{
  Buffer::NodeList nodes;
  const size_t init_cap = Buffer::NodeList::InitialCapacity;
  EXPECT_EQ(init_cap, nodes.capacity());
  EXPECT_EQ(0u, nodes.size());
  nodes.reserve(2 * init_cap);
  EXPECT_EQ(2 * init_cap, nodes.capacity());
  EXPECT_EQ(0u, nodes.size());

  nodes.reserve(init_cap);
  EXPECT_EQ(2 * init_cap, nodes.capacity());  // doesn't shrink
  EXPECT_EQ(0u, nodes.size());

  for (size_t n = 0; n < 2 * init_cap; ++n) {
    EXPECT_EQ(0, nodes[n].data);
    EXPECT_EQ(0u, nodes[n].used);
  }
  EXPECT_EQ(&nodes[0], nodes.raw_data());

  nodes.reserve(3 * init_cap);
  EXPECT_EQ(3 * init_cap, nodes.capacity());
  EXPECT_EQ(0u, nodes.size());
  for (size_t n = 0; n < 3 * init_cap; ++n) {
    EXPECT_EQ(0, nodes[n].data);
    EXPECT_EQ(0u, nodes[n].used);
  }
  EXPECT_EQ(&nodes[0], nodes.raw_data());
}

TEST(NodeList, emplace_back)
{
  Buffer::NodeList nodes;

  char* s1 = static_cast<char*>(::malloc(32));
  nodes.emplace_back(s1, 0);
  EXPECT_EQ(s1, nodes[0].data);
  EXPECT_EQ(0u, nodes[0].used);
  char* s2 = static_cast<char*>(::malloc(64));
  nodes.emplace_back(s2, 0);
  EXPECT_EQ(s2, nodes[1].data);
  EXPECT_EQ(0u, nodes[1].used);

  ::free(s1); ::free(s2);
}

TEST(NodeList, swap)
{
  const size_t init_cap = Buffer::NodeList::InitialCapacity;

  Buffer::NodeList nodes, nodes1;
  char* s1 = static_cast<char*>(::malloc(32));
  nodes.emplace_back(s1, 0);

  nodes1.swap(nodes);
  EXPECT_EQ(init_cap, nodes.capacity());
  EXPECT_EQ(0u, nodes.size());
  EXPECT_EQ(init_cap, nodes1.capacity());
  EXPECT_EQ(1u, nodes1.size());
  EXPECT_EQ(s1, nodes1[0].data);

  char* s2 = static_cast<char*>(::malloc(64));
  nodes1.emplace_back(s2, 0);
  nodes1.swap(nodes);
  EXPECT_EQ(init_cap, nodes1.capacity());
  EXPECT_EQ(0u, nodes1.size());
  EXPECT_EQ(init_cap, nodes.capacity());
  EXPECT_EQ(2u, nodes.size());
  EXPECT_EQ(s1, nodes[0].data);
  EXPECT_EQ(s2, nodes[1].data);

  char* s3 = static_cast<char*>(::malloc(32));
  nodes1.emplace_back(s3, 0);
  nodes.swap(nodes1);
  EXPECT_EQ(1u, nodes.size());
  EXPECT_EQ(2u, nodes1.size());
  EXPECT_EQ(s1, nodes1[0].data);
  EXPECT_EQ(s2, nodes1[1].data);
  EXPECT_EQ(s3, nodes[0].data);

  ::free(s1); ::free(s2); ::free(s3);
}

TEST(NodeList, append)
{
  Buffer::NodeList nodes, nodes1;
  char* s1 = static_cast<char*>(::malloc(32));
  nodes.emplace_back(s1, 0);
  char* s2 = static_cast<char*>(::malloc(64));
  nodes1.emplace_back(s2, 0);
  nodes.append(nodes1);
  EXPECT_EQ(2u, nodes.size());
  EXPECT_EQ(s1, nodes[0].data);
  EXPECT_EQ(s2, nodes[1].data);
  nodes.emplace_back(nullptr, 0);
  nodes.emplace_back(nullptr, 0);
  EXPECT_EQ(4u, nodes.size());

  ::free(s1); ::free(s2);
}

/// Buffer ///
//
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
  std::string s(buf.base_size() / 2, 'a');
  buf.append(s.c_str(), s.size());
  EXPECT_EQ(s.size(), buf.size());

  buf.append(s.c_str(), s.size());
  EXPECT_EQ(2 * s.size(), buf.size());
  EXPECT_EQ(std::string(buf.size(), 'a'), to_str(buf));

  buf.reserve(2 * buf.base_size());
  buf.append('a');
  EXPECT_EQ(2 * s.size() + 1, buf.size());
  EXPECT_EQ(std::string(buf.size(), 'a'), to_str(buf));
  buf.append("a", 1);
  EXPECT_EQ(2 * s.size() + 2, buf.size());
  EXPECT_EQ(std::string(buf.size(), 'a'), to_str(buf));
}

TEST(Buffer, append_data)
{
  Buffer buf;
  buf.reserve(2 * buf.base_size());
  char data[] = "0123456789";
  std::string s1;
  for (int i = 0; i < 20; ++i)
    s1.append(data);
  buf.append(s1.c_str(), s1.size());
  EXPECT_EQ(200u, buf.size());
  EXPECT_EQ(s1, to_str(buf));
}

TEST(Buffer, swap)
{
  Buffer buf;
  std::string s1(buf.base_size() / 2, 'a');
  buf.append(s1.c_str(), s1.size());

  Buffer buf1;
  buf1.swap(buf);
  EXPECT_EQ(s1.size(), buf1.size());
  EXPECT_EQ(0u, buf.size());
  EXPECT_EQ(std::string(), to_str(buf));
  EXPECT_EQ(s1, to_str(buf1));

  std::string s2(buf.base_size() + 1, 'b');
  buf.append(s2.c_str(), s2.size());
  buf.swap(buf1);
  EXPECT_EQ(s1.size(), buf.size());
  EXPECT_EQ(s2.size(), buf1.size());
  EXPECT_EQ(s1, to_str(buf));
  EXPECT_EQ(s2, to_str(buf1));
}

TEST(Buffer, reclaim)
{
  Buffer buf;
  EXPECT_EQ(0u, buf.capacity());
  std::string s1(1 + buf.base_size() / 2, 'a');
  buf.append(s1.c_str(), s1.size());
  EXPECT_EQ(buf.base_size(), buf.capacity());
  EXPECT_EQ(s1.size(), buf.size());
  buf.reclaim();
  EXPECT_EQ(0u, buf.size());
  EXPECT_EQ(buf.base_size(), buf.capacity());

  buf.append(s1.c_str(), s1.size());
  EXPECT_EQ(buf.base_size(), buf.capacity());
  EXPECT_EQ(s1.size(), buf.size());
}

TEST(Buffer, combine_buffer)
{
  Buffer buf, buf1;
  std::string s1(buf.base_size() / 2, 'a');
  buf.append(s1.c_str(), s1.size());
  std::string s2(buf.base_size() / 2 + 1, 'b');
  buf1.append(s2.c_str(), s2.size());

  buf.combine_buffer(buf1);
  EXPECT_EQ(buf.base_size() + 1, buf.size());
}

