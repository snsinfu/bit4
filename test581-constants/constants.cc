#include <iomanip>
#include <iostream>

#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>


int main()
{
    using real = boost::multiprecision::cpp_dec_float_50;

    auto const pi = boost::math::constants::pi<real>();
    auto const reciprocal_sqrt2pi = sqrt(1 / (2 * pi));
    auto const two = real(2);
    auto const reciprocal_sqrt2 = sqrt(1 / two);
    auto const tau = 2 * pi;

    std::cout << std::setprecision(50);
    std::cout << "1/√2π\t" << reciprocal_sqrt2pi << '\n';
    std::cout << "1/√2\t" << reciprocal_sqrt2 << '\n';
    std::cout << "2pi\t" << tau << '\n';
}
