#include <string>
#include <utest.hpp>
#include <ku/lua/lua.hpp>

using namespace ku::lua;

TEST(lua, get)
{
  std::string str = R"(
    var1 = 3.14
    var2 = true
    var3 = "string"
    )";
  Lua lua;
  EXPECT_TRUE(lua.load(str.c_str()));
  EXPECT_EQ(3.14, lua.get<double>("var1"));
  EXPECT_EQ(true, lua.get<bool>("var2"));
  EXPECT_EQ(std::string("string"), lua.get<std::string>("var3"));
}

