#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <utility>


int main()
{
    // Broad-phase neighbor search in a 2D periodic box
    //
    // We divide the periodic box into a grid. We make the side of a grid cell
    // larger than or equal to dneigh, the neighbor distance. This way, all
    // the neighbor points of a point (center point) is contained in the cell
    // containing the center point and its surrounding cells.
    //
    //  +--+--+--+--+--+--+--+
    //  |  |  |//|//|//|  |  |
    //  +--+--+--+--+--+--+--+
    //  |  |  |//|**|//|  |  |    ** center point
    //  +--+--+--+--+--+--+--+    // surrounding cells
    //  |  |  |//|//|//|  |  |
    //  +--+--+--+--+--+--+--+

    constexpr double width = 2;
    constexpr double height = 1;
    constexpr double dneigh = 0.07;
    constexpr std::size_t npoints = 10000;
    constexpr std::size_t nbuckets = 392;
    constexpr std::uint32_t x_prime = 1;
    constexpr std::uint32_t y_prime = 28;

    // The cells need to be uniform for our neighbor search strategy to work.
    // However, dneigh does not always divide the box width and height. So we
    // "round up" dneigh to a valid value. Note that the cell does not need to
    // be square as long as the sides are not shorter than dneigh.

    auto const x_interval = width / std::floor(width / dneigh);
    auto const y_interval = height / std::floor(height / dneigh);

    assert(x_interval >= dneigh);
    assert(y_interval >= dneigh);

    auto const x_cells = int(std::rint(width / x_interval));
    auto const y_cells = int(std::rint(height / y_interval));

    assert(std::fabs(x_cells * x_interval - width) < 1e-6);
    assert(std::fabs(y_cells * y_interval - height) < 1e-6);

    // Generate random points in the box.

    struct point {
        double x = 0;
        double y = 0;
    };
    std::vector<point> points;

    std::mt19937 random;
    std::uniform_real_distribution<double> x_dist{0+dneigh, width-dneigh};
    std::uniform_real_distribution<double> y_dist{0+dneigh, height-dneigh};

    std::generate_n(std::back_inserter(points), npoints, [&] {
        return point{x_dist(random), y_dist(random)};
    });

    // Construct buckets.

    struct bucket {
        std::vector<std::size_t> members;
        std::vector<std::size_t> neighbors;
    };
    std::vector<bucket> buckets(nbuckets);

    auto const hash_coords = [=](int x, int y) {
        std::uint64_t hash = 0;
        hash += std::uint64_t(x) * x_prime;
        hash += std::uint64_t(y) * y_prime;
        hash %= nbuckets;
        return static_cast<std::size_t>(hash);
    };

    for (std::size_t bucket_index = 0; bucket_index < nbuckets; bucket_index++) {
        std::set<std::size_t> neighbors;

        for (int const dx : {-1, 0, 1}) {
            for (int const dy : {-1, 0, 1}) {
                auto const hx = (dx + x_cells) % x_cells;
                auto const hy = (dy + y_cells) % y_cells;
                neighbors.insert((bucket_index + hash_coords(hx, hy)) % nbuckets);
            }
        }

        buckets[bucket_index].neighbors.assign(neighbors.begin(), neighbors.end());
    }

    // Assign points to the buckets.

    auto const truncate = [=](double value) {
        return static_cast<int>(value);
    };

    for (std::size_t point_index = 0; point_index < points.size(); point_index++) {
        auto const x = truncate(points[point_index].x / x_interval);
        auto const y = truncate(points[point_index].y / y_interval);
        auto const bucket_index = hash_coords(x, y);
        buckets[bucket_index].members.push_back(point_index);
    }

    // Neighbor search. Scan buckets and their surrounding buckets, iterating
    // over the pairs of contained points.

    std::set<std::pair<std::size_t, std::size_t>> found_pairs;

    auto const distance = [=](point const& p1, point const& p2) {
        // Distance is computed for the nearest periodic images.
        auto const rx = std::remainder(p1.x - p2.x, width);
        auto const ry = std::remainder(p1.y - p2.y, height);
        return std::hypot(rx, ry);
    };

    for (auto const& bucket : buckets) {
        for (auto const neighbor_index : bucket.neighbors) {
            auto const& neighbor = buckets[neighbor_index];

            for (auto const i : bucket.members) {
                for (auto const j : neighbor.members) {
                    //// Prevent double counting by enforcing i < j.
                    //if (i >= j) {
                    //    continue;
                    //}
                    //assert(i < j);
                    if (i == j) {
                        continue;
                    }

                    if (distance(points[i], points[j]) < dneigh) {
                        // found_pairs.emplace(i, j);
                        found_pairs.emplace(std::min(i, j), std::max(i, j));
                    }
                }
            }
        }
    }

    std::clog << "Found " << found_pairs.size() << " neighbor pairs\n";

    // Brute-force search for validation.

    std::set<std::pair<std::size_t, std::size_t>> expected_pairs;

    for (std::size_t j = 0; j < points.size(); j++) {
        for (std::size_t i = 0; i < j; i++) {
            if (distance(points[i], points[j]) < dneigh) {
                expected_pairs.emplace(i, j);
            }
        }
    }

    std::clog << "Expecting " << expected_pairs.size() << " neighbor pairs\n";

    assert(found_pairs == expected_pairs);
}
