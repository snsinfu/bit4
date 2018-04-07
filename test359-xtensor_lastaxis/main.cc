#include <iostream>
#include <utility>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xtensor.hpp>


template<typename E>
auto sum_last_axis(E&& expr)
{
    return xt::sum(std::forward<E>(expr), {expr.dimension() - 1});
}


int main()
{
    xt::xtensor<double, 1> t1({5}, 0.0);
    xt::xtensor<double, 2> t2({10, 5}, 0.0);
    xt::xtensor<double, 3> t3({20, 10, 5}, 0.0);

    auto s1 = sum_last_axis(t1);
    auto s2 = sum_last_axis(t2);
    auto s3 = sum_last_axis(t3);

    std::cout << s1 << '\n';
    std::cout << s2 << '\n';
    std::cout << s3 << '\n';
}
