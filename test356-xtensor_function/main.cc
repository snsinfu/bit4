#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "beta.hpp"


template<typename T>
T make_copy(T const& obj)
{
    return obj;
}

template<typename E>
auto log_beta(E&& e)
{
    return xt::sum(xt::lgamma(make_copy(e)), {1})
           - xt::lgamma(xt::sum(make_copy(e), {1}));
}

int main()
{
    xt::xtensor<double, 2> X({10, 5});
    X = 1 + xt::random::rand<double>(X.shape());
    auto Y = log_beta(X + X);

    std::cout << Y << '\n';
}
