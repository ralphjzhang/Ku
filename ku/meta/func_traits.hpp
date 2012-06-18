#pragma once
#include "variadic.hpp"

namespace ku { namespace meta {

template <typename R, typename... Args>
struct func_traits;

template <typename R, typename... Args>
struct func_traits<R(Args...)>
{
  static const size_t arity = sizeof...(Args);
  using result_type = R;
  template <size_t N> struct arg
  {
    using type = typename ku::meta::at<N - 1, Args...>::type;
  };
};

} } // namespace ku::meta

