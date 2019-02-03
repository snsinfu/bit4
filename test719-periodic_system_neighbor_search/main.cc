#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include "math.hpp"
#include "vec.hpp"


using std::size_t;
using std::uint32_t;


int main()
{
    // x: periodic
    // y: unbounded

    const size_t n_points = 1000;
    const double x_span = 1.0;
    const double y_offset = 1.0e6;
    const double dcut = 0.11;

    const double x_min = -0.5;
    const double x_max = 1.5;
    const double y_min = -5.0;
    const double y_max = 5.0;

    // Generate points

    std::vector<vec> points;
    std::mt19937 random;

    std::generate_n(std::back_inserter(points), n_points, [&] {
        std::uniform_real_distribution x_coord{x_min, x_max};
        std::uniform_real_distribution y_coord{y_min, y_max};
        return vec{x_coord(random), y_coord(random)};
    });

    // Estimate density

    double y_mean = 0;
    double y_meansq = 0;

    for (const auto& point : points) {
        y_mean += point.y;
        y_meansq += point.y * point.y;
    }

    y_mean /= n_points;
    y_meansq /= n_points;

    const auto y_stddev = std::sqrt(y_meansq - y_mean * y_mean);
    const auto est_y_span = 3 * y_stddev;
    const auto est_density = n_points / (x_span * est_y_span);

    // Define the hash table parameters

    const auto x_bins = fasttrunc_uint(x_span / dcut);
    const auto x_dcut = x_span / static_cast<double>(x_bins);
    const auto y_dcut = dcut;

    const auto min_occupancy = 5.0; // Too small occupancy degrades performance
    const auto est_occupancy = est_density * x_dcut * y_dcut;
    const auto plan_occupancy = std::max(est_occupancy, min_occupancy);

    const auto plan_n_buckets = static_cast<uint32_t>(n_points / plan_occupancy);
    const auto y_bins = (plan_n_buckets + x_bins - 1) / x_bins;

    std::clog << "est_occupancy: " << est_occupancy << '\n';
    std::clog << "plan_occupancy: " << plan_occupancy << '\n';

    const auto n_buckets = x_bins * y_bins;

    assert(x_dcut >= dcut);
    assert(y_dcut >= dcut);

    std::clog << "x_bins: " << x_bins << '\n';
    std::clog << "y_bins: " << y_bins << '\n';

    auto const hash_coords = [&](uint32_t x, uint32_t y) {
        return x + x_bins * (y % y_bins);
    };

    // Assign points to the hash table

    struct spatial_bucket
    {
        struct member
        {
            vec point;
            uint32_t index;
        };
        std::vector<member> members;
    };

    std::vector<spatial_bucket> buckets(n_buckets);

    for (uint32_t index = 0; index < n_points; index++) {
        const auto point = points[index];

        const auto x = fastmod(point.x, x_span, 1 / x_span);
        const auto y = point.y + y_offset;

        assert(x >= 0);
        assert(y >= 0);

        const auto x_bin = fasttrunc_uint(x / x_dcut);
        const auto y_bin = fasttrunc_uint(y / y_dcut);
        const auto bucket_index = hash_coords(x_bin, y_bin);

        buckets[bucket_index].members.push_back({point, index});
    }

    std::clog << "\noccupancy:\n";

    int col = 0;
    for (const auto& bucket : buckets) {
        std::clog << bucket.members.size() << "\t\n"[++col % 8 == 0];
    }
    if (col % 8 != 0) {
        std::clog << '\n';
    }

    double mean_occupancy = 0;
    for (const auto& bucket : buckets) {
        mean_occupancy += static_cast<double>(bucket.members.size());
    }
    mean_occupancy /= n_buckets;
    std::clog << "\nmean_occupancy: " << mean_occupancy << '\n';
}
