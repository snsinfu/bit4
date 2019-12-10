#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

#include "vec.hpp"


vec              generate_vec(std::mt19937& random);
std::vector<vec> generate_path(int n, double sigma, double wobble, std::mt19937& random);
std::vector<vec> oversample_path(std::vector<vec> const& path, int rate);
vec              determine_back_direction(std::vector<vec> const& path, std::size_t i);
vec              determine_forw_direction(std::vector<vec> const& path, std::size_t i);


int main()
{
    constexpr int    path_length = 8;
    constexpr double sigma       = 0.2;
    constexpr double wobble      = 2.0;
    constexpr int    over_rate   = 4;

    std::mt19937 random;

    auto const path = generate_path(path_length, sigma, wobble, random);
    auto const over_path = oversample_path(path, over_rate);

    for (auto const pt : over_path) {
        std::cout << pt.x << '\t' << pt.y << '\t' << pt.z << '\n';
    }
}


vec generate_vec(std::mt19937& random)
{
    std::normal_distribution<double> normal;
    return normalize({normal(random), normal(random), normal(random)});
}


std::vector<vec> generate_path(int n, double sigma, double wobble, std::mt19937& random)
{
    std::vector<vec> path;
    vec dir;

    path.push_back({0, 0, 0});

    for (int i = 1; i < n; i++) {
        dir = normalize(dir + wobble * generate_vec(random));
        path.push_back(path.back() + sigma * dir);
    }

    return path;
}


std::vector<vec> oversample_path(std::vector<vec> const& path, int rate)
{
    assert(rate % 2 == 0);
    auto const semi_rate = rate / 2;

    std::vector<vec> over_path;

    for (std::size_t i = 0; i < path.size(); i++) {
        auto const back_vec = determine_back_direction(path, i);
        auto const forw_vec = determine_forw_direction(path, i);

        for (int k = 0; k < semi_rate; k++) {
            auto const pos = path[i] + (0.5 * (semi_rate - k) / semi_rate) * back_vec;
            over_path.push_back(pos);
        }

        for (int k = 0; k < semi_rate; k++) {
            auto const pos = path[i] + (0.5 * k / semi_rate) * forw_vec;
            over_path.push_back(pos);
        }
    }

    return over_path;
}


vec determine_back_direction(std::vector<vec> const& path, std::size_t i)
{
    if (i == 0) {
        return -1 * determine_forw_direction(path, i);
    }
    return path[i - 1] - path[i];
}


vec determine_forw_direction(std::vector<vec> const& path, std::size_t i)
{
    if (i + 1 == path.size()) {
        return -1 * determine_back_direction(path, i);
    }
    return path[i + 1] - path[i];
}
