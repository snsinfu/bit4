#ifndef INCLUDED_SIMULATOR_HPP
#define INCLUDED_SIMULATOR_HPP

#include <algorithm>
#include <iterator>
#include <random>
#include <vector>


struct simulator
{
    simulator()
    {
        std::uniform_real_distribution<double> coord_dist{-5, 5};
        std::generate_n(
            std::back_inserter(coords_),
            200000,
            [&] { return coord_dist(random_); }
        );
    }

    void step()
    {
        std::normal_distribution<double> normal{0, 0.01};
        for (double& coord : coords_) {
            coord += normal(random_);
        }
    }

    std::vector<double> const& coords() const
    {
        return coords_;
    }

private:
    std::vector<double> coords_;
    std::mt19937 random_;
};

#endif
