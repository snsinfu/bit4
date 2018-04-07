#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "beta.hpp"
#include "expr.hpp"


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
