#ifndef INCLUDED_MATH_HPP
#define INCLUDED_MATH_HPP

#include <cmath>
#include <type_traits>
#include <utility>

#include <xtensor/xfunction.hpp>
#include <xtensor/xmath.hpp>


// Computes the logarithm of the beta function applied to the last axis of
// given tensor.
template<typename E>
auto log_beta(E&& expr);

// Computes the digamma function applied to given expression.
template<typename E>
auto digamma(E&& expr);

//------------------------------------------------------------------------------

template<typename E>
auto log_beta(E&& expr)
{
    using closure_type = xt::const_xclosure_t<E>;
    closure_type& closure = expr;

    return xt::sum(xt::lgamma(closure_type{closure}), {expr.dimension() - 1})
            - xt::lgamma(xt::sum(closure_type{closure}, {expr.dimension() - 1}));
}

namespace detail
{
    inline double digamma(double x)
    {
        constexpr double epsilon = 1e-6;
        constexpr double euler = 0.5772156649015328606;
        constexpr double min_stable_x = 6.0;

        if (x < epsilon) {
            return -euler - 1 / x;
        }

        double result = 0;

        // digamma(x) = digamma(x+1) - 1/x
        while (x < min_stable_x) {
            result -= 1 / x;
            x += 1;
        }

        // digamma(x) = log(x) - 1/(2x) - 1/(12x^2) + 1/(120x^4) - 1/(252x^6) + ...
        double r = 1 / x;
        result += std::log(x) - 0.5 * r;
        r *= r;
        result += r * (-1./12 + r * (1./120 + r * (-1./252)));

        return result;
    }

    template<typename T>
    struct digamma_functor
    {
        T operator()(T arg) const
        {
            return digamma(arg);
        }
    };
}

template<typename E>
auto digamma(E&& expr)
{
    using value_type = xt::xvalue_type_t<std::decay_t<E>>;
    using functor_type = detail::digamma_functor<value_type>;
    using xfunction_type = xt::xfunction<functor_type, value_type, E>;
    return xfunction_type{functor_type{}, std::forward<E>(expr)};
}

#endif
