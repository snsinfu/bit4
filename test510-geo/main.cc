#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "geo.hpp"
#include "neighbor_searcher.hpp"


int main()
{
    using point_t = geo::point<double, 3>;
    using vector_t = geo::vector<double, 3>;

    std::vector<point_t> points;

    std::mt19937_64 engine;
    std::uniform_real_distribution<point_t::scalar_type> uniform{-1, 1};
    std::normal_distribution<point_t::scalar_type> normal;

    {
        point_t point;

        for (std::size_t i = 0; i < 60000; ++i) {
            if (i % 2000 == 0) {
                point += 5 * vector_t{
                    uniform(engine),
                    uniform(engine),
                    uniform(engine)
                };
            }

            points.push_back(point);

            point += 0.01 * vector_t{
                normal(engine),
                normal(engine),
                normal(engine)
            };
        }
    }

    geo::neighbor_searcher<double> neighbors{10000, 0.1};
    std::vector<std::pair<std::size_t, std::size_t>> pairs;
    pairs.reserve(10000000);

    neighbors.set_points(points);
    neighbors.search(std::back_inserter(pairs));

    std::cout << pairs.size() << '\n';
}
