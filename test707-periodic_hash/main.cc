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
    // Broad-phase neighbor search in a 2D periodic box, with hashed bins
    // instead of grid bins. Compare with test702.

    constexpr double width = 2;
    constexpr double height = 1;
    constexpr double dneigh = 0.07;
    constexpr std::size_t npoints = 10000;
    constexpr std::size_t nbuckets = 2000;
    constexpr std::uint32_t x_prime = 860278967;
    constexpr std::uint32_t y_prime = 817926563;

    // We again utilize the concept of uniform grid cells. This time we hash
    // cells into buckets. Hashing greatly improves performance if points are
    // sparse or dimension is >= 3.

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
    std::uniform_real_distribution<double> x_dist{0, width};
    std::uniform_real_distribution<double> y_dist{0, height};

    std::generate_n(std::back_inserter(points), npoints, [&] {
        return point{x_dist(random), y_dist(random)};
    });

    // Construct buckets. Each bucket contains the indices of the points and
    // the indices of "surrounding" buckets. Here bucket A is surrounded by
    // bucket B when a cell in the bucket A is surrounded by one or more cells
    // in the bucket B. We use a linear hash so that this relation is preserved
    // on hashing.

    struct bucket {
        std::vector<std::size_t> members;
        std::vector<std::size_t> neighbors;
    };
    std::vector<bucket> buckets(nbuckets);

    auto const linear_hash = [=](int x, int y) {
        std::uint64_t hash = 0;
        hash += std::uint64_t(x) * x_prime;
        hash += std::uint64_t(y) * y_prime;
        hash %= nbuckets;
        return static_cast<std::size_t>(hash);
    };

    for (std::size_t bucket_index = 0; bucket_index < nbuckets; bucket_index++) {
        std::set<std::size_t> neighbors;

        for (int const dx : {int(nbuckets) - 1, 0, 1}) {
            for (int const dy : {int(nbuckets) - 1, 0, 1}) {
                neighbors.insert((bucket_index + linear_hash(dx, dy)) % nbuckets);
            }
        }

        buckets[bucket_index].neighbors.assign(neighbors.begin(), neighbors.end());
    }

    // Validate symmetry of the neighborhood relation.

    for (std::size_t bucket_index = 0; bucket_index < nbuckets; bucket_index++) {
        for (std::size_t neighbor_index : buckets[bucket_index].neighbors) {
            auto const it = std::find(
                buckets[neighbor_index].neighbors.begin(),
                buckets[neighbor_index].neighbors.end(),
                bucket_index
            );
            assert(it != buckets[neighbor_index].neighbors.end());
        }
    }

    // Assign points to the buckets.

    auto const truncate = [=](double value) {
        return static_cast<int>(value);
    };

    for (std::size_t point_index = 0; point_index < points.size(); point_index++) {
        auto const x = truncate(points[point_index].x / x_interval);
        auto const y = truncate(points[point_index].y / y_interval);
        auto const bucket_index = linear_hash(x, y);
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
                    if (i < j && distance(points[i], points[j]) < dneigh) {
                        found_pairs.emplace(i, j);
                    }
                }
            }
        }
    }

    std::clog << "Found " << found_pairs.size() << " neighbor pairs\n";

    // Brute-force search for validation.

    std::set<std::pair<std::size_t, std::size_t>> expected_pairs;

    for (std::size_t i = 0; i < points.size(); i++) {
        for (std::size_t j = 0; j < points.size(); j++) {
            if (i < j && distance(points[i], points[j]) < dneigh) {
                expected_pairs.emplace(i, j);
            }
        }
    }

    std::clog << "Expecting " << expected_pairs.size() << " neighbor pairs\n";

    assert(found_pairs == expected_pairs);
}
