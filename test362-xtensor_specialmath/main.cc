#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <utility>

#include <xtensor/xexpression.hpp>
#include <xtensor/xfunction.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "math.hpp"


namespace
{
    // copy creates a copy of given object.
    template<typename T>
    T copy(T const& obj)
    {
        return obj;
    }

    // copy_expr creates a copy of given xtensor expression. This function does
    // not create a deep copy of an xcontainer but creates a new view.
    template<typename E>
    auto copy_expr(E const& expr)
    {
        return +expr;
    }

    // log_beta computes the logarithm of the beta function applied to the last
    // axis of given tensor.
    template<typename E>
    auto log_beta(E&& expr)
    {
        std::array<std::size_t, 1> const axes{expr.dimension() - 1};
        return xt::sum(xt::lgamma(copy_expr(expr)), copy(axes))
                - xt::lgamma(xt::sum(copy_expr(expr), copy(axes)));
    }

    template<typename T>
    struct digamma_functor
    {
        T operator()(T arg) const
        {
            return math::digamma(arg);
        }
    };

    // digamma computes the digamma function.
    template<typename E>
    auto digamma(E&& expr)
    {
        using value_type = xt::xvalue_type_t<std::decay_t<E>>;
        using functor_type = digamma_functor<value_type>;
        using xfunction_type = xt::xfunction<functor_type, value_type, E>;
        return xfunction_type{functor_type{}, std::forward<E>(expr)};
    }

    template<typename E, std::size_t... Indices>
    auto append_newaxis_impl(E&& expr, std::index_sequence<Indices...>)
    {
        return xt::view(std::forward<E>(expr),
                        ((void) Indices, xt::all())..., xt::newaxis());
    }

    // append_newaxis creates a view of given expression with an extra
    // dimension appended after the last axis.
    template<typename E,
             std::size_t N = std::tuple_size<typename std::decay_t<E>::shape_type>::value>
    auto append_newaxis(E&& expr)
    {
        std::make_index_sequence<N> index_sequence;
        return append_newaxis_impl(std::forward<E>(expr), index_sequence);
    }

    // dirichlet_log_expect computes the expectation of the logarithm of
    // Dirichlet variables with given parameters on the last axis.
    template<typename E>
    auto dirichlet_log_expect(E&& params)
    {
        std::array<std::size_t, 1> const axis{params.dimension() - 1};
        return digamma(copy_expr(params))
            - append_newaxis(digamma(xt::sum(copy_expr(params), copy(axis))));
    }
}

int main()
{
    xt::xtensor<double, 2> X({5, 2});
    X = 1 + xt::random::rand<double>(X.shape());

    std::cout << log_beta(X) << '\n';
    std::cout << log_beta(2 * X) << '\n';

    xt::xtensor<double, 2> Y = dirichlet_log_expect(X);
    assert(X.shape() == Y.shape());

    std::cout << Y << '\n';
}
