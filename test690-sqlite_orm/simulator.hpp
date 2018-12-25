#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

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
        std::generate_n(
            std::back_inserter(coords),
            200000,
            [&] { return coord_dist(random); }
        );
    }

    void step()
    {
        std::normal_distribution<double> normal{0, 0.01};
        for (double& coord : coords) {
            coord += normal(random);
        }
    }
};

#endif
