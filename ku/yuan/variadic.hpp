#pragma once
#include <type_traits>

namespace ku { namespace yuan {

template <typename F, typename Head, typename... Tail>
struct apply_each
{
  void operator () ( Head, Tail... )
  {
  }
};

/// head<>::type
//
template <typename Head, typename... Tail>
struct head
{
  typedef Head type;
};

/// length<>::value
//
template <typename... Tl>
struct length
{
  enum { value = sizeof...(Tl) };
};

/// at<>::type
//
template <size_t N, typename... Tl>
struct at
{
  static_assert(sizeof...(Tl) > 0, "Index out of range in at<>.");
};

template <size_t N, typename Head, typename... Tail>
struct at<N, Head, Tail...>
{
  typedef typename at<N - 1, Tail...>::type type;
};
template <typename Head, typename... Tail>
struct at<0, Head, Tail...>
{
  typedef Head type;
};

/// find<>::value 
//
template <typename T, typename... Tl > struct find;
template <typename T>
struct find<T>
{
  enum { value = -1 };
};
template <typename T, typename Head, typename... Tail>
struct find<T, Head, Tail...>
{
  enum { value = std::is_same<T, Head>::value ? 0 : find<T, Tail...>::value };
};
/*
// append<>::type
//
template <typename Tl, typename Tp> struct append;
template <typename Tp, typename... Ti>
struct append<typelist<Ti...>, Tp>
{
  typedef typelist<Ti..., Tp> type;
};

// insert<>::type
//
template <typename Tl, typename Tp> struct insert;
template <typename Tp, typename... Ti>
struct insert<typelist<Ti...>, Tp>
{
  typedef typelist<Tp, Ti...> type;
};

// remove<>::type
//
template <typename Tl, typename Tp> struct remove;
template <typename Tp>
struct remove<typelist<>, Tp>
{
  typedef typelist<> type;
};
template <typename Tp, typename... Ti>
struct remove<typelist<Tp, Ti...>, Tp>
{
  typedef typelist<Ti...> type;
};
template <typename Tp, typename Th, typename... Ti>
struct remove<typelist<Th, Ti...>, Tp>
{
  typedef typelist<Th, typename remove<typelist<Ti...>, Tp>::type> type;
};
*/

} } // namespace ku::yuan

