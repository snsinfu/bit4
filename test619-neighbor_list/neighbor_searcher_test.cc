#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "catch.hpp"
#include "md.hpp"


TEST_CASE("neighbor_searcher")
{
    md::scalar const dcut = 0.2;

    std::vector<md::point> points;
    {
        std::mt19937 random;
        std::normal_distribution<md::scalar> dist;

        std::generate_n(std::back_inserter(points), 1000, [&] {
            return md::point{dist(random), dist(random), dist(random)};
        });
    }

    md::neighbor_searcher searcher{dcut, points.size() / 20};
    searcher.set_points(points);
    std::vector<std::pair<md::index, md::index>> actual;
    searcher.search(std::back_inserter(actual));
    std::sort(actual.begin(), actual.end());

    std::vector<std::pair<md::index, md::index>> expected;
    for (md::index j = 0; j < points.size(); j++) {
        for (md::index i = 0; i < j; i++) {
            if (md::distance(points[i], points[j]) < dcut) {
                expected.emplace_back(i, j);
            }
        }
    }
    std::sort(expected.begin(), expected.end());

    CHECK(std::includes(actual.begin(), actual.end(), expected.begin(), expected.end()));
    CHECK(std::includes(expected.begin(), expected.end(), actual.begin(), actual.end()));

    CHECK(actual.size() == expected.size());
    CHECK(actual.at(100) == expected.at(100));

    std::vector<std::pair<md::index, md::index>> diff;
    std::set_difference(expected.begin(), expected.end(), actual.begin(), actual.end(), std::back_inserter(diff));
    for (auto p : diff) {
        std::clog << p.first << ':' << p.second << '\t';

        auto pi = points[p.first];
        auto pj = points[p.second];
        std::clog << md::distance(pi, pj) << '\t';

        std::clog << pi << " [" << searcher.locate_bin(pi) << "]\t";
        std::clog << pj << " [" << searcher.locate_bin(pj) << "]\n";
    }
}
