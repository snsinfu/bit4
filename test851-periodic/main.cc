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

    open_box box{points.size()};
    neighbor_searcher<open_box> searcher{box, 0.1};
    searcher.set_points(points);
    std::vector<std::pair<md::index, md::index>> pairs;
    searcher.search(std::back_inserter(pairs));
    std::cout << pairs.size() << '\n';
}
