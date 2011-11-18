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

} } // namespace ku::util

