#ifndef INCLUDED_DELTA_CODER_HPP
#define INCLUDED_DELTA_CODER_HPP

#include <cmath>
#include <cstddef>
#include <vector>


struct delta_coder
{
    std::vector<int> const& operator()(std::vector<double> const& coords)
    {
        delta_coords_.resize(coords.size());
        prev_coords_.resize(coords.size());

        for (std::size_t i = 0; i < coords.size(); i++) {
            int const x = int(std::round(coords[i] / resolution_));
            delta_coords_[i] = x - prev_coords_[i];
            prev_coords_[i] = x;
        }

        return delta_coords_;
    }

private:
    std::vector<int> delta_coords_;
    std::vector<int> prev_coords_;
    double resolution_ = 0.001;

};

#endif
