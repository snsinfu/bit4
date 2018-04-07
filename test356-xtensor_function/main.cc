#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "beta.hpp"


template<typename E>
auto log_beta(E&& e)
{
    return xt::sum(xt::lgamma(e), {1}) - xt::lgamma(xt::sum(e, {1}));
}

template<typename E>
auto beta(E&& e)
{
    return xt::exp(log_beta(e));
}

int main()
{
    xt::xtensor<double, 2> X({10, 5});
    X = 1 + xt::random::rand<double>(X.shape());
    auto Y = beta(X + X);

    std::cout << Y << '\n';
}
