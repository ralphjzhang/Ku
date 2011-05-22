#pragma once

#include <tuple>
#include <utility>


namespace ku { namespace su {

namespace detail {

template<typename Tuple, typename F, size_t N>
struct tuple_for_each_unroll
{
    auto operator()(Tuple const& t, F f) -> void
    {
        detail::tuple_for_each_unroll<Tuple, F, N-1>()(t, f);
        f(std::get<N>(t));
    }
};

template<typename Tuple, typename F>
struct tuple_for_each_unroll<Tuple, F, 0>
{
    auto operator()(Tuple const& t, F f) -> void
    {
        f(std::get<0>(t));
    }
};

}


template <typename Tuple, typename F>
inline auto
tuple_for_each(Tuple const& tup, F f) -> void
{
    detail::tuple_for_each_unroll<Tuple, F, std::tuple_size<Tuple>::value-1>()(tup, f);
}


} } // namespace ku::su

