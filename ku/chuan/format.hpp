#pragma once
#include <type_traits>
#include <tuple>
#include <utility>
#include <functional>
#include "chuan.hpp"

namespace ku { namespace chuan {

using std::enable_if;

template <typename T>
struct placeholder
{
  enum { value = std::is_placeholder<T>::value };
  placeholder(T) { }

  template <typename Iter, typename... Args>
  inline Iter apply( Iter iter, std::tuple<Args...> const& t ) const
  {
    return chuan(iter, std::get<value - 1>(t));
  }
};

template <typename T>
struct string
{
  T const& value;
  string(T const& value) : value(value) { }

  template <typename Iter, typename... Args>
  inline Iter apply( Iter iter, std::tuple<Args...> const& ) const
  {
    return chuan(iter, value);
  }
};

template <typename T, typename U>
struct lit
{
  T value;
  U next;
  lit( T const& value, U const& next ) : value(value), next(next) { }
  lit( lit const& rhs ) : value(rhs.value), next(rhs.next) { }

  template <typename Iter, typename... Args>
  inline Iter apply( Iter iter, std::tuple<Args...> const& t ) const
  {
    return next.apply(value.apply(iter, t), t);
  }
};

template <typename T>
struct lit<T, void>
{
  T value;
  lit( T const& value ) : value(value) { }

  template <typename Iter, typename... Args>
  inline Iter apply( Iter iter, std::tuple<Args...> const& t ) const
  {
    return value.apply(iter, t);
  }
};

template <typename T, size_t N>
inline auto to_lit( T(&t)[N] ) -> string<decltype(t)> 
{
  return string<decltype(t)>(t);
}

template <typename T>
inline auto to_lit( T t )
    -> typename enable_if<(std::is_placeholder<T>::value > 0), placeholder<T>>::type
{
  return placeholder<T>(t);
}

template <typename T, size_t N>
auto make_lit( T(&t)[N] ) -> lit<decltype(to_lit(t)), void>
{
  return lit<decltype(to_lit(t)), void>(to_lit(t));
}

template <typename T>
auto make_lit( T const& t, typename enable_if<(std::is_placeholder<T>::value > 0)>::type* = nullptr )
    -> lit<decltype(to_lit(t)), void>
{
  return lit<decltype(to_lit(t)), void>(to_lit(t));
}

template <typename U, typename V, typename T>
inline auto operator | ( lit<U, V> const& f, T t ) -> lit<lit<U, V>, placeholder<T>>
{
  return lit<lit<U, V>, placeholder<T>>(f, to_lit(t));
}

template <typename U, typename V, typename T, size_t N>
inline auto operator | ( lit<U, V> const& f, T(&t)[N] ) -> lit<lit<U, V>, decltype(to_lit(t))>
{
  return lit<lit<U, V>, decltype(to_lit(t))>(f, to_lit(t));
}

template <typename Iter, typename T, typename U, typename... Args>
Iter format(Iter iter, lit<T, U> const& f, Args const&... args...)
{
  auto arg_tuple = std::make_tuple(args...);
  return f.apply(iter, arg_tuple);
}

} } // namespace ku::chuan

