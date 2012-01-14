#pragma once
#include <tuple>
#include <utility>

namespace ku { namespace util {

namespace aux {

template<typename Tuple, typename F, size_t N>
struct tuple_for_each_unroll
{
  void operator()(Tuple const& t, F f)
  {
    tuple_for_each_unroll<Tuple, F, N - 1>()(t, f);
    f(std::get<N>(t));
  }
};

template<typename Tuple, typename F>
struct tuple_for_each_unroll<Tuple, F, 0>
{
  void operator()(Tuple const& t, F f) { f(std::get<0>(t)); }
};

} // namespace ku::util::aux

template <typename Tuple, typename F>
inline void tuple_for_each(Tuple const& tup, F f)
{
  aux::tuple_for_each_unroll<Tuple, F, std::tuple_size<Tuple>::value - 1>()(tup, f);
}

namespace aux {

template <size_t...>
struct indices;

template <size_t N, typename Indices, typename... Types>
struct make_indices_impl;

template <size_t N, size_t... Indices, typename Type, typename Types...>
struct make_indices_impl<N, indices<Indices...>, Type, Types...>
{
  typedef typename make_indices_impl<N + 1, indices<Indices..., N>, Types...>::type type;
};

template <size_t N, size_t...Indices>
struct make_indices_impl<N, indices<Indices...>>
{
  typedef indices<Indices...> type;
};

} // namespace ku::util::aux

template <size_t N, typename... Types>
struct make_indices
{
  typedef typename aux::make_indices_impl<0, aux::indices<>, Types...>::type type;
};

} } // namespace ku::util

