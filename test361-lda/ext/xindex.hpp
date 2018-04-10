#ifndef INCLUDED_XINDEX_HPP
#define INCLUDED_XINDEX_HPP

#include <cstddef>
#include <utility>

#include <xtensor/xstrided_view.hpp>
#include <xtensor/xview.hpp>


namespace ext
{
    // Creates a view of given expression with an extra dimension appended
    // after the last axis.
    template<typename E,
             std::size_t N = std::tuple_size<typename std::decay_t<E>::shape_type>::value>
    auto append_newaxis(E&& expr);
}

namespace ext
{
    template<typename E, std::size_t... Indices>
    auto append_newaxis_impl(E&& expr, std::index_sequence<Indices...>)
    {
        return xt::view(std::forward<E>(expr),
                        ((void) Indices, xt::all())..., xt::newaxis());
    }

    template<typename E, std::size_t N>
    auto append_newaxis(E&& expr)
    {
        std::make_index_sequence<N> index_sequence;
        return append_newaxis_impl(std::forward<E>(expr), index_sequence);
    }
}

#endif
