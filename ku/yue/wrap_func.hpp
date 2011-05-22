#pragma once
#include <ku/yue/extract_arg.hpp>
#include <ku/yue/push_result.hpp>

namespace ku { namespace yue {

namespace aux {
struct decrementor
{
  size_t n;
  decrementor(size_t n) : n(n) {}
  size_t operator ()( ) { return n--; }
};
} // namespace aux

template <typename Sig, Sig& Func>
struct wrap_func
{
  static_assert(sizeof(Sig), "Invalid parameter T, must be a function type.");
};

template <typename R, typename... Args, R(&Func)(Args...)>
struct wrap_func<R(Args...), Func>
{
  typedef R(function_type)(Args...);
  static function_type& function()
  {
    return Func;
  }

  static int call( lua_State *L )
  {
    size_t n = sizeof...(Args);
    aux::decrementor decr(n);
    // TODO this relies on parameter evaluation reverse order to work, not good
    push_result(
        L, Func(extract_arg<Args>(L, decr())...)
        );
    return 1;
  }
};

} } // namespace ku::yue

