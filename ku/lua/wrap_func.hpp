#pragma once
#include "extract_arg.hpp"

namespace ku { namespace lua {

namespace aux {
struct decrementor
{
  size_t n;
  decrementor(size_t n) : n(n) {}
  size_t operator()(){ return n--; }
};
} // namespace aux

/// push_result pushes result into a lua stack
//
void push_result(lua_State *L, double result)
{
  lua_pushnumber(L, result);
}


/// wrap_func wraps normal C functions into lua-compatible signature
//
template <typename Sig, Sig& Func>
struct wrap_func;

template <typename R, typename... Args, R(&Func)(Args...)>
struct wrap_func<R(Args...), Func>
{
  using function_type = R()(Args...);
  static function_type& function()
  { return Func; }

  static int call(lua_State *L)
  {
    push_result(
        L, Func(extract_arg<Args, sizeof...(Args)>()(L)...)
        );
    return 1;
  }
};

} } // namespace ku::lua

