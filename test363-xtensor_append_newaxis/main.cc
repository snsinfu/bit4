#include <iostream>
#include <type_traits>
#include <utility>

#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>


namespace
{
    template<typename E, std::size_t... Indices>
    auto append_newaxis_impl(E&& expr, std::index_sequence<Indices...>)
    {
        return xt::view(expr, ((void) Indices, xt::all())..., xt::newaxis());
    }

    template<typename E,
             std::size_t N = std::tuple_size<typename std::decay_t<E>::shape_type>::value>
    auto append_newaxis(E&& expr)
    {
        std::make_index_sequence<N> index_sequence;
        return append_newaxis_impl(std::forward<E>(expr), index_sequence);
    }
}

int main()
{
    xt::xtensor<double, 1> t1({2}, 1.0);
    xt::xtensor<double, 2> t2({2, 3}, 2.0);
    xt::xtensor<double, 2> r = append_newaxis(t1) + t2;

    std::cout << r << '\n';
}
