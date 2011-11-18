#include <stdio.h>
#include <math.h>
#include <ku/yue/yue.hpp>

int func0()
{
  printf("this is func0\n");
}

double func2(double d, const char* s)
{
  printf("this is func2: %s\n", s);
  return sin(d);
}

template <typename Sig>
constexpr Sig& getfunc(Sig& Func)
{
  return Func;
}

int cfunc2(lua_State *L)
{
  getfunc(func2);
  ku::yue::wrap_func<decltype(func2), (getfunc(func2))>::call(L);
  return 1;
}

extern "C" int luaopen_yue(lua_State *L)
{
  using namespace ku::yue;
  static Yue lua(L, "yue"
      , wrap_func<decltype(func0), func0>()
      , wrap_func<decltype(func2), func2>()
      );
  return 1;
}

