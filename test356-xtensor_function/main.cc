#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "beta.hpp"


int main()
{
    xt::xtensor<double, 2> X({10, 5});

    X = xt::random::rand<double>(X.shape());

    auto log_beta = xt::sum(xt::lgamma(X), {1}) - xt::lgamma(xt::sum(X, {1}))

    std::cout << log_beta << '\n';
}
