#ifndef DELTA_CODER_HPP
#define DELTA_CODER_HPP

#include <cmath>
#include <cstddef>
#include <vector>


struct delta_coder
{
    std::vector<int> delta_coords;
    std::vector<int> prev_coords;

    double resolution = 0.001;

    std::vector<int> const& operator()(std::vector<double> const& coords)
    {
        delta_coords.resize(coords.size());
        prev_coords.resize(coords.size());

        for (std::size_t i = 0; i < coords.size(); i++) {
            int const x = int(std::round(coords[i] / resolution));
            delta_coords[i] = x - prev_coords[i];
            prev_coords[i] = x;
        }

        return delta_coords;
    }
};

#endif
