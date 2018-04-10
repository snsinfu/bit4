#ifndef INCLUDED_XUTILITY_HPP
#define INCLUDED_XUTILITY_HPP

#include <type_traits>

#include <xtensor/xexpression.hpp>


namespace ext
{
    // Creates a copy of given xtensor expression. This function does not
    // create a deep copy of an xcontainer but creates a new view.
    template<typename E,
             std::enable_if_t<std::is_base_of<xt::xexpression<E>, E>::value, int> = 0>
    auto copy_expr(E const& expr)
    {
        return +expr;
    }
}

#endif
