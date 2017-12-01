// https://github.com/QuantStack/xtensor
// https://xtensor.readthedocs.io/en/latest/numpy.html

#include <iostream>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xstrided_view.hpp>

int main()
{
    xt::xarray<double> points = stack(xtuple(
        xt::linspace<double>(1.0, 4.0, 100),
        xt::linspace<double>(2.0, 3.0, 100),
        xt::linspace<double>(4.0, 2.0, 100)
    ));
    points = transpose(points); // Undocumented API?

    points += 0.01 * xt::random::randn<double>(points.shape());
    points -= mean(points);

    // sum() returns xreducer with dimension zero. Use operator() to evaluate.
    double radius_of_gyration = sqrt(sum(points * points) / 6)();
    std::cout << radius_of_gyration << '\n';
}
