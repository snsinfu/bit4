#include <algorithm>
#include <cmath>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "catch.hpp"
#include "md.hpp"
#include "quasirandom.hpp"


TEST_CASE("neighbor_list")
{
    md::scalar const dcut = 0.2;

    std::vector<md::point> points;
    {
        constexpr double phi_3 = 1.220744084605759475361685349108831;

        kronecker_sequence x{0, 1 / phi_3};
        kronecker_sequence y{0, 1 / (phi_3 * phi_3)};
        kronecker_sequence z{0, 1 / (phi_3 * phi_3 * phi_3)};

        std::generate_n(std::back_inserter(points), 1000, [&] {
            return md::point{x(), y(), z()};
        });
    }

    md::neighbor_list nlist{1.3, 20};

    nlist.update(points, dcut);
    {
        std::mt19937 random;
        std::normal_distribution<md::scalar> normal{0, dcut};
        for (auto& point : points) {
            point += {normal(random), normal(random), normal(random)};
        }

        nlist.update(points, dcut);
    }

    std::vector<std::pair<md::index, md::index>> actual;
    std::copy_if(nlist.begin(), nlist.end(), std::back_inserter(actual), [&](auto pair) {
        return md::distance(points[pair.first], points[pair.second]) < dcut;
    });
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

    CHECK(actual.size() == expected.size());
    CHECK(actual.at(100) == expected.at(100));
}
