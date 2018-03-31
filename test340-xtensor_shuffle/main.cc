#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

int main()
{
    xt::xtensor<double, 2> u({1, 2});
    xt::xtensor<double, 2> v({3, 1});
    xt::xtensor<double, 3> w({1, 2, 3});

    // Now let us compute w[i,j,k] = u[i,j] * v[k,i].

    // view
    w = xt::view(u, xt::all(), xt::all(), xt::newaxis())
        * xt::view(xt::transpose(v), xt::all(), xt::newaxis(), xt::all());

    // expand_dims
    w = xt::expand_dims(u, 2) * xt::expand_dims(xt::transpose(v), 1);
}
