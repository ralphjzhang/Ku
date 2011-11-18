#pragma once
#include <lua.hpp>

namespace ku { namespace lua {

/// extract_arg extracts argument from a lua stack
//
template <typename T>
T extract_arg(lua_State *L, size_t n)
{
  static_assert(sizeof(T), "extract_arg for this type not implemented.");
}

template <>
double extract_arg<double>(lua_State *L, size_t n)
{
  return luaL_checknumber(L, n);
};

template <>
char const* extract_arg<char const*>(lua_State *L, size_t n)
{
  return luaL_checkstring(L, n);
};

} } // namespace ku::lua

