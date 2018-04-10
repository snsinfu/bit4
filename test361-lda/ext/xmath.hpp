#ifndef INCLUDED_XMATH_HPP
#define INCLUDED_XMATH_HPP

#include <array>
#include <cstddef>

#include <xtensor/xcontainer.hpp>
#include <xtensor/xfunction.hpp>
#include <xtensor/xmath.hpp>

#include "copy.hpp"
#include "math.hpp"
#include "xindex.hpp"
#include "xutility.hpp"


namespace ext
{
    // Computes the logarithm of the beta function applied to the last axis of
    // given tensor.
    template<typename E>
    auto log_beta(E&& expr);

    // Computes the digamma function applied to given expression.
    template<typename E>
    auto digamma(E&& expr);
}

namespace ext
{
    template<typename E>
    auto log_beta(E && expr)
    {
        std::array<std::size_t, 1> const axis{expr.dimension() - 1};
        return xt::sum(xt::lgamma(copy_expr(expr)), copy(axis))
                - xt::lgamma(xt::sum(copy_expr(expr), copy(axis)));
    }

    template<typename T>
    struct digamma_functor
    {
        T operator()(T arg) const
        {
            return math::digamma(arg);
        }
    };

    template<typename E>
    auto digamma(E&& expr)
    {
        using value_type = xt::xvalue_type_t<std::decay_t<E>>;
        using functor_type = digamma_functor<value_type>;
        using xfunction_type = xt::xfunction<functor_type, value_type, E>;
        return xfunction_type{functor_type{}, std::forward<E>(expr)};
    }
}

#endif
