#pragma once
#include <type_traits>

namespace ku { namespace net { namespace util {

// TODO forwarding
template <typename T, typename... Args>
auto create_object(Args... args)
  -> decltype(new T(args...))
{
  return new T(args...);
}

template <typename T, typename... Args>
auto create_object(Args... args)
  -> decltype(T::create(args...))
{
  return T::create(args...);
}

} } } // namespace ku::net::util

