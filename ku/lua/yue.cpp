#include <stdio.h>
#include <math.h>
#include "yue.hpp"

namespace ku { namespace lua {

void Yue::register_funcs(lua_State *L)
{
  size_t size = funcs_.size();
  luaL_Reg *map = new luaL_Reg[size + 1];
  map[size] = {NULL, NULL};
  for (int i = 0; i < size; ++i) {
    map[i] = {names_[i].c_str(), funcs_[i]};
  }
  ::luaL_setfuncs(L, map, 0);
  delete[] map;
}

} } // namespace ku::lua


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
  //ku::lua::wrap_func<decltype(func2), (getfunc(func2))>::call(L);
  return 1;
}

extern "C" int luaopen_yue(lua_State *L)
{
  using namespace ku::lua;
  static Yue lua(L, "yue"
      , wrap_func<decltype(func0), func0>()
      , wrap_func<decltype(func2), func2>()
      );
  return 1;
}

