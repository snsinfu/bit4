#include <iostream>
#include <vector>

#include "geo/all.hpp"

int main()
{
    using point_t = geo::point<double, 3>;

    point_t p = {1, 2, 3};
    point_t q = {4, 5, 6};

    std::cout << distance(p, q) << '\n';
    std::cout << interpolate(p, q, 0.2) << '\n';
}
