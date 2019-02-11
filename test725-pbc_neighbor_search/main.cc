#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include <point.hpp>


using std::int32_t;
using std::uint32_t;


namespace detail
{
    double floor_mod(double x, double m)
    {
        return x - std::floor(x * (1 / m)) * m;
    }

    double round_mod(double x, double m)
    {
        return x - std::nearbyint(x * (1 / m)) * m;
    }

    uint32_t trunc_uint(double x)
    {
        return static_cast<uint32_t>(static_cast<int32_t>(x));
    }

    uint32_t round_uint(double x)
    {
        return trunc_uint(std::nearbyint(x));
    }
}


int main()
{
    const uint32_t n_points = 10000;
    const cxx::vector period = {1.0, 2.3, 3.0};
    const double dcut = 0.13;

    struct bin_layout
    {
        double span;
        double step;
        uint32_t count;
    };

    const auto define_bin = [&](double span, double dcut) {
        bin_layout bin;
        bin.span = span;
        bin.step = span / std::trunc(span / dcut);
        bin.count = detail::round_uint(span / bin.step);
        return bin;
    };

    const auto x_bin = define_bin(period.x, dcut);
    const auto y_bin = define_bin(period.y, dcut);
    const auto z_bin = define_bin(period.z, dcut);

    const auto locate_bin = [&](const bin_layout& bin, double coord) {
        const auto image = detail::floor_mod(coord, bin.span);
        const auto bin_index =  detail::trunc_uint(image * (1 / bin.step));
        return bin_index < bin.count ? bin_index : bin.count - 1;
    };

    std::vector<cxx::point> points;
    std::mt19937 random;
    std::generate_n(std::back_inserter(points), n_points, [&] {
        std::uniform_real_distribution<double> coord{-2.4, 4.5};
        return cxx::point{coord(random), coord(random), coord(random)};
    });

    struct spatial_bucket
    {
        struct member
        {
            cxx::point point;
            uint32_t index;
        };
        std::vector<member> members;
        std::vector<uint32_t> adjacents;
    };

    const auto locate_bucket = [&](uint32_t x, uint32_t y, uint32_t z) {
        return x + x_bin.count * (y + y_bin.count * z);
    };

    const auto add_mod = [&](uint32_t x, uint32_t dx, uint32_t mod) {
        x += dx;
        if (x >= mod) {
            x -= mod;
        }
        return x;
    };

    const auto n_buckets = x_bin.count * y_bin.count * z_bin.count;
    std::vector<spatial_bucket> buckets(n_buckets);

    for (uint32_t z = 0; z < z_bin.count; z++) {
        for (uint32_t y = 0; y < y_bin.count; y++) {
            for (uint32_t x = 0; x < x_bin.count; x++) {
                const auto index = locate_bucket(x, y, z);
                auto& bucket = buckets[index];

                for (uint32_t dz : {0u, 1u, z_bin.count - 1}) {
                    for (uint32_t dy : {0u, 1u, y_bin.count - 1}) {
                        for (uint32_t dx : {0u, 1u, x_bin.count - 1}) {
                            const auto adj_x = add_mod(x, dx, x_bin.count);
                            const auto adj_y = add_mod(y, dy, y_bin.count);
                            const auto adj_z = add_mod(z, dz, z_bin.count);
                            const auto adj_index = locate_bucket(adj_x, adj_y, adj_z);
                            if (adj_index <= index) {
                                bucket.adjacents.push_back(adj_index);
                            }
                        }
                    }
                }

                auto& adjacents = bucket.adjacents;
                std::sort(adjacents.begin(), adjacents.end());
                adjacents.erase(
                    std::unique(adjacents.begin(), adjacents.end()),
                    adjacents.end()
                );
            }
        }
    }

    for (uint32_t index = 0; index < points.size(); index++) {
        const auto point = points[index];
        const auto x = locate_bin(x_bin, point.x);
        const auto y = locate_bin(y_bin, point.y);
        const auto z = locate_bin(z_bin, point.z);
        auto& bucket = buckets[locate_bucket(x, y, z)];
        bucket.members.push_back({point, index});
    }

    const auto mod_squared_distance = [&](cxx::point p1, cxx::point p2) {
        const auto dx = detail::round_mod(p1.x - p2.x, period.x);
        const auto dy = detail::round_mod(p1.y - p2.y, period.y);
        const auto dz = detail::round_mod(p1.z - p2.z, period.z);
        return dx * dx + dy * dy + dz * dz;
    };

    const auto dcut_sq = dcut * dcut;
    std::vector<std::pair<uint32_t, uint32_t>> neighbors;

    for (const auto& bucket : buckets) {
        for (const auto adj_index : bucket.adjacents) {
            const auto& adj_bucket = buckets[adj_index];

            for (const auto member_i : bucket.members) {
                for (const auto member_j : adj_bucket.members) {
                    if (member_i.index == member_j.index) {
                        // Avoid double counting
                        break;
                    }

                    if (mod_squared_distance(member_i.point, member_j.point) < dcut_sq) {
                        const auto i = std::min(member_i.index, member_j.index);
                        const auto j = std::max(member_i.index, member_j.index);
                        neighbors.emplace_back(i, j);
                    }
                }
            }
        }
    }

    const auto colhash = [](uint32_t i, uint32_t j) {
        return (~i * j) ^ (i * ~j);
    };

    uint32_t actual_cols = 0;
    uint32_t actual_colhash = 0;
    for (const auto& pair : neighbors) {
        actual_colhash += colhash(pair.first, pair.second);
        actual_cols++;
    }
    std::cout << actual_cols << '\t' << actual_colhash << '\n';

    uint32_t expected_cols = 0;
    uint32_t expected_colhash = 0;
    for (uint32_t i = 0; i < points.size(); i++) {
        for (uint32_t j = 0; j < i; j++) {
            if (mod_squared_distance(points[i], points[j]) < dcut_sq) {
                expected_colhash += colhash(i, j);
                expected_cols++;
            }
        }
    }
    std::cout << expected_cols << '\t' << expected_colhash << '\n';

    assert(actual_cols == expected_cols);
    assert(actual_colhash == expected_colhash);
}
