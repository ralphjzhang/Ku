#pragma once
#include <lua.hpp>

namespace ku { namespace yue {

/// push_result pushes result into a lua stack
//
template <typename T>
void push_result( lua_State *L, T&& result )
{
  static_assert(sizeof(T), "push_result for this type not implemented.");
}

template <>
void push_result<double>( lua_State *L, double&& result )
{
  lua_pushnumber(L, result);
}

} } // namespace ku::yue

