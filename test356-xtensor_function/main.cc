#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <type_traits>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "beta.hpp"


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

template<typename E>
auto log_beta(E&& e)
{
    return xt::sum(xt::lgamma(copy_expr(e)), {1})
           - xt::lgamma(xt::sum(copy_expr(e), {1}));
}

int main()
{
    xt::xtensor<double, 2> X({10, 5});
    X = 1 + xt::random::rand<double>(X.shape());
    auto Y = log_beta(X + X);
    std::cout << Y << '\n';

    double dummy = 0;
    for (long i = 0; i < 10000000; ++i) {
        auto ex = copy_expr(X);
        dummy += ex(0, 0);
    }
    std::cout << dummy << '\n';
}
