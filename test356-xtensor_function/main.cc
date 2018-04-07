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


template<typename E>
auto log_beta(E&& e)
{
    std::decay_t<E> e1 = e;
    std::decay_t<E> e2 = e;
    return xt::sum(xt::lgamma(std::move(e1)), {1})
           - xt::lgamma(xt::sum(std::move(e2), {1}));
}

int main()
{
    xt::xtensor<double, 2> X({10, 5});
    X = 1 + xt::random::rand<double>(X.shape());
    auto Y = log_beta(X + X);

    std::cout << Y << '\n';
}
