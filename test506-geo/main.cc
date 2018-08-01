#include <iostream>
#include <vector>

#include "geo.hpp"


int main()
{
    using point_t = geo::point<double, 3>;
    using vector_t = point_t::vector_type;

    point_t p = {1, 2, 3};
    point_t q = {4, 5, 6};

    std::cout << p.distance(q) << '\n';
    std::cout << (p - q).normalize() << '\n';

    p += vector_t{5, 4, 3};

    std::cout << p.distance(q - vector_t{2, 1, 0}) << '\n';
}
