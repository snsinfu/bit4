#ifndef INCLUDED_XEXPR_HPP
#define INCLUDED_XEXPR_HPP

#include <type_traits>

#include <xtensor/xcontainer.hpp>


template<typename E,
         std::enable_if_t<std::is_base_of<xt::xcontainer<E>, E>::value, int> = 0>
auto copy_expr(E const& expr)
{
    return +expr;
}

template<typename E,
         std::enable_if_t<!std::is_base_of<xt::xcontainer<E>, E>::value, int> = 0>
auto copy_expr(E const& expr)
{
    return expr;
}

#endif
