#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include "point.hpp"


namespace
{
    // Computes fmod(x, m) quickly. m_inv must be the inverse of m.
    inline double floor_mod(double x, double m, double m_inv)
    {
        return x - std::floor(x * m_inv) * m;
    }

    // Truncates non-negative value x to unsigned integer. On x86-64 this is faster
    // than just casting x to unsigned.
    inline unsigned trunc_uint(double x)
    {
        // On x86-64 float-to-unsigned conversion is slow.
        return static_cast<unsigned>(static_cast<int>(x));
    }

    // Computes standard deviation of z coordinate values of given points.
    inline double compute_z_stddev(const std::vector<cxx::point>& points)
    {
        double mean = 0;
        double meansq = 0;

        for (const auto& point : points) {
            mean += point.z;
            meansq += point.z * point.z;
        }

        mean /= static_cast<double>(points.size());
        meansq /= static_cast<double>(points.size());

        return std::sqrt(meansq - mean * mean);
    }
}


class xy_periodic_neighbor_searcher
{
public:
    using uint32_t = std::uint32_t;

    xy_periodic_neighbor_searcher(
        const std::vector<cxx::point>& points,
        double dcut,
        cxx::vector period
    )
        : period_(period)
        , dcut_(dcut)
    {
        // Heuristic
        const auto x_span = period_.x;
        const auto y_span = period_.y;

        const auto x_bins = std::max(trunc_uint(x_span / dcut), 1u);
        const auto x_dcut = x_span / x_bins;
        const auto y_bins = std::max(trunc_uint(y_span / dcut), 1u);
        const auto y_dcut = y_span / y_bins;
        const auto z_dcut = dcut;

        const auto est_z_span = 3 * compute_z_stddev(points);
        const auto est_volume = x_span * y_span * est_z_span;
        const auto est_density = points.size() / est_volume;

        const auto min_occupancy = 4.0;
        const auto est_occupancy = x_dcut * y_dcut * z_dcut * est_density;
        const auto plan_occupancy = std::max(est_occupancy, min_occupancy);

        const auto plan_n_buckets = uint32_t(points.size() / plan_occupancy);
        const auto xy_bins = x_bins * y_bins;
        const auto z_bins = (plan_n_buckets + xy_bins - 1) / xy_bins;
        const auto n_buckets = xy_bins * z_bins;

        assert(x_bins > 0);
        assert(y_bins > 0);
        assert(z_bins > 0);
        std::clog << x_bins << '\t' << y_bins << '\t' << z_bins << '\n';

        buckets_.resize(n_buckets);
    }

private:
    struct spatial_bucket
    {
        struct member
        {
            cxx::point point;
            uint32_t index;
        };
        std::vector<member> members;
        std::vector<uint32_t> neighbors;
    };

    std::vector<double> buckets_;
    cxx::vector period_;
    double dcut_ = 0;
};


int main()
{
    std::mt19937 random;

    std::uniform_real_distribution<double> x_coord{-1.0, 1.0};
    std::uniform_real_distribution<double> y_coord{-2.0, 2.0};
    std::uniform_real_distribution<double> z_coord{-5.0, 5.0};
    const cxx::vector period = {2.0, 4.0, 0};
    const double dcut = 0.1;

    std::vector<cxx::point> points;
    std::generate_n(std::back_inserter(points), 2000, [&] {
        return cxx::point{x_coord(random), y_coord(random), z_coord(random)};
    });

    xy_periodic_neighbor_searcher searcher{points, dcut, period};
}
