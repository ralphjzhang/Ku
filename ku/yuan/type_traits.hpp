#pragma once
#include "variadic.hpp"

namespace ku { namespace yuan {

template <typename F>
struct is_template
{
  enum { value = false };
};

template <template<typename...> class T, typename... Tl>
struct is_template<T<Tl...>>
{
  enum { value = true };
};

} } // namespace ku::yuan

