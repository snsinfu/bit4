#ifndef INCLUDED_DIGAMMA_HPP
#define INCLUDED_DIGAMMA_HPP

#include <cmath>
#include <utility>

#include <xtensor/xexpression.hpp>
#include <xtensor/xfunction.hpp>


namespace detail
{
    double digamma(double x)
    {
        constexpr double epsilon = 1e-6;
        constexpr double euler = 0.5772156649015328606;
        constexpr double min_stable_x = 6;

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
        result -= r * (1./12 - r * (1./120 - r * (1./252)));

        return result;
    }
}

template<typename T>
struct digamma_fun
{
    T operator()(T arg) const
    {
        return detail::digamma(arg);
    }
};

template<typename Expr>
auto digamma(Expr&& expr)
{
    using value_type = xt::xvalue_type_t<std::decay_t<Expr>>;
    using functor_type = digamma_fun<value_type>;

    return xt::xfunction<functor_type, value_type, Expr>{
        functor_type{}, std::forward<Expr>(expr)
    };
}

#endif
