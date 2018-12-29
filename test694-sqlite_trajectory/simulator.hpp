#ifndef INCLUDE_SIMULATOR_HPP
#define INCLUDE_SIMULATOR_HPP

#include <algorithm>
#include <iterator>
#include <random>
#include <vector>


struct simulator
{
    std::vector<double> coords;
    std::mt19937 random;

    simulator()
    {
        std::uniform_real_distribution<double> coord_dist{-5, 5};

        for (int i = 0; i < 10000; i++) {
            coords.push_back(i * 0.001);
        }
    }

    void step()
    {
        std::normal_distribution<double> normal_dist{0, 0.0001};
        double prev_normal = 0;

        for (double& coord : coords) {
            auto const normal = normal_dist(random);
            coord += (normal + prev_normal) / 2;
            prev_normal = normal;
        }
    }
};

#endif
