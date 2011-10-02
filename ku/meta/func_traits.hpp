#pragma once
#include "variadic.hpp"

namespace ku { namespace meta {

template <typename R, typename... Args>
struct func_traits;

template <typename R, typename... Args>
struct func_traits<R(Args...)>
{
  static const size_t arity = sizeof...(Args);
  typedef R result_type;
  template <size_t N> struct arg
  {
    typedef typename ku::meta::at<N - 1, Args...>::type type;
  };
};

} } // namespace ku::meta

