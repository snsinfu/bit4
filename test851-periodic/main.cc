#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include <md.hpp>

#include "neighbor_searcher.hpp"


int main()
{
    std::vector<md::point> points;

    std::mt19937_64 random;
    std::uniform_real_distribution<md::scalar> coord{0, 1};
    for (int i = 0; i < 1000; i++) {
        points.push_back({ coord(random), coord(random), coord(random) });
    }

    std::vector<std::pair<md::index, md::index>> pairs;

    pairs.clear();
    neighbor_searcher<open_box> open_searcher{open_box{points.size()}, 0.1};
    open_searcher.set_points(points);
    open_searcher.search(std::back_inserter(pairs));
    std::cout << pairs.size() << '\n';

    pairs.clear();
    neighbor_searcher<periodic_box> periodic_searcher{periodic_box{1, 1, 1}, 0.1};
    periodic_searcher.set_points(points);
    periodic_searcher.search(std::back_inserter(pairs));
    std::cout << pairs.size() << '\n';
}
