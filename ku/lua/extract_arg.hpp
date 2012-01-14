#pragma once
#include <lua.hpp>

namespace ku { namespace lua {

/// extract_arg extracts argument from a lua stack
//
template <typename T, size_t N>
struct extract_arg;

template <size_t N>
struct extract_arg<double, N>
{
  double operator()(lua_State *L)
  {
    return luaL_checknumber(L, N);
  }
};

template <size_t N>
struct extract_arg<char const*, N>
{
  char const* operator()(lua_State *L)
  {
    return luaL_checkstring(L, N);
  }
};

} } // namespace ku::lua

