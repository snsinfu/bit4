#include <iostream>
#include <random>
#include <vector>

#include "h5.hpp"


struct vec
{
    double x = 0;
    double y = 0;
    double z = 0;
};


int main()
{
    h5::file file{"test.h5"};

    std::mt19937_64 random;
    std::vector<vec> points;
    for (int i = 0; i < 1000; i++) {
        std::normal_distribution<double> normal;
        points.push_back({normal(random), normal(random), normal(random)});
    }

    auto data = file.dataset<float, 2>("/data/points");
    data.write(
        reinterpret_cast<double const*>(points.data()),
        {points.size(), 3},
        {.compression = 5, .scaleoffset = 3}
    );
}
