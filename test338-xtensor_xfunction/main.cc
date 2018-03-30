#include <iostream>
#include <utility>

#include <xtensor/xarray.hpp>
#include <xtensor/xfunction.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>


class increment_fun
{
  public:
    double operator()(double arg) const
    {
        return arg + 1;
    }
};

template<typename E>
auto increment(E&& e)
{
    return xt::xfunction<increment_fun, double, E>{increment_fun{}, std::forward<E>(e)};
}

int main()
{
    xt::xtensor<double, 3> src({3, 4, 5});
    xt::xtensor<double, 3> dest(src.shape());

    dest = increment(src);

    std::cout << dest << '\n';
}
