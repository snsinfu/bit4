#include <bitset>
#include <chrono>
#include <ios>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <boost/container/small_vector.hpp>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include <glm/gtx/norm.hpp>

#include <hhdate/date.h>


struct CollisionHash
{
    void feed(std::size_t i, std::size_t j)
    {
        hash_ += static_cast<std::uint64_t>(i * j);
        count_ += 1;
    }

    std::uint64_t value() const noexcept
    {
        return hash_ * count_;
    }

    std::uint64_t count() const noexcept
    {
        return count_;
    }

  private:
    std::uint64_t hash_ = 0;
    std::uint64_t count_ = 0;
};

std::ostream& operator<<(std::ostream& os, CollisionHash const& hash)
{
    return os << std::dec << hash.count() << ':'
              << std::hex << hash.value()
              << std::dec;
}


CollisionHash brute_force(std::vector<glm::dvec2> const& points, double radius)
{
    double const dcut = 2 * radius;
    double const dcut2 = dcut * dcut;

    CollisionHash col_hash;

    std::vector<std::pair<std::size_t, std::size_t>> collisions;
    collisions.reserve(points.size() * 9);

    for (std::size_t j = 0; j < points.size(); ++j) {
        for (std::size_t i = 0; i < j; ++i) {
            if (glm::distance2(points[i], points[j]) < dcut2) {
                col_hash.feed(i, j);
            }
        }
    }

    return col_hash;
}


CollisionHash sweep_hash(std::vector<glm::dvec2> const& points, double radius)
{
    struct SweepPoint
    {
        double      hash;
        std::size_t index;
    };

    double const dcut = 2 * radius;
    double const dcut2 = dcut * dcut;
    double const bin_freq = 1 / radius;

    // IEEE 754 double can store exact integers less than 2^53.
    double const stride = 1 << 26;
    double const offset = stride / 2;

    std::vector<SweepPoint> sweep;
    sweep.reserve(points.size() * 9);

    for (std::size_t i = 0; i < points.size(); ++i) {

        glm::dvec2 const point = points[i];
        double const x = std::round(bin_freq * point.x) + offset;
        double const y = std::round(bin_freq * point.y) + offset;

        for (double dx = -1; dx <= 1; ++dx) {
            for (double dy = -1; dy <= 1; ++dy) {
                double const hash = (x + dx) + (y + dy) * stride;
                sweep.push_back(SweepPoint{hash, i});
            }
        }

    }

    std::sort(sweep.begin(), sweep.end(), [](SweepPoint const& p, SweepPoint const& q) {
        return p.hash < q.hash;
    });

    std::vector<std::pair<std::size_t, std::size_t>> collisions;
    collisions.reserve(sweep.size());

    for (std::size_t group_beg = 0; group_beg < sweep.size(); ) {

        double const group_hash = sweep[group_beg].hash;

        std::size_t group_end = group_beg;
        for (; group_end < sweep.size(); ++group_end) {
            if (sweep[group_end].hash != group_hash) {
                break;
            }
        }

        for (std::size_t iter_j = group_beg; iter_j < group_end; ++iter_j) {
            for (std::size_t iter_i = group_beg; iter_i < iter_j; ++iter_i) {
                std::size_t const i = sweep[iter_i].index;
                std::size_t const j = sweep[iter_j].index;

                if (glm::distance2(points[i], points[j]) < dcut2) {
                    collisions.emplace_back(std::min(i, j),
                                            std::max(i, j));
                }
            }
        }

        group_beg = group_end;
    }

    std::sort(collisions.begin(), collisions.end());
    collisions.erase(std::unique(collisions.begin(), collisions.end()),
                     collisions.end());

    CollisionHash col_hash;
    for (auto const& ij : collisions) {
        col_hash.feed(ij.first, ij.second);
    }
    return col_hash;
}


CollisionHash uniform_bins(std::vector<glm::dvec2> const& points, double radius)
{
    struct Bin
    {
        boost::container::small_vector<std::size_t, 8> indices;
    };

    double const dcut = 2 * radius;
    double const dcut2 = dcut * dcut;
    double const bin_freq = 1 / dcut;

    glm::dvec2 min_point = points.front();
    glm::dvec2 max_point = points.front();
    for (glm::dvec2 const& point : points) {
        min_point.x = std::min(min_point.x, point.x);
        min_point.y = std::min(min_point.y, point.y);
        max_point.x = std::max(max_point.x, point.x);
        max_point.y = std::max(max_point.y, point.y);
    }

    double const x_stride = max_point.x - min_point.x;
    double const y_stride = max_point.y - min_point.y;

    std::size_t const nx = static_cast<std::size_t>(bin_freq * x_stride) + 1;
    std::size_t const ny = static_cast<std::size_t>(bin_freq * y_stride) + 1;

    std::vector<Bin> bins(nx * ny);

    for (std::size_t i = 0; i < points.size(); ++i) {
        glm::dvec2 const point = points[i];

        std::size_t const x = static_cast<std::size_t>(bin_freq * (point.x - min_point.x));
        std::size_t const y = static_cast<std::size_t>(bin_freq * (point.y - min_point.y));

        bins[x + y * nx].indices.push_back(i);
    }

    CollisionHash col_hash;

    for (std::size_t x = 0; x < nx; ++x) {
        for (std::size_t y = 0; y < ny; ++y) {

            std::size_t const center = x + y * nx;

            // within-case
            std::size_t const n_center = bins[center].indices.size();
            for (std::size_t iter_i = 0; iter_i < n_center; ++iter_i) {
                for (std::size_t iter_j = 0; iter_j < n_center; ++iter_j) {
                    std::size_t const i = bins[center].indices[iter_i];
                    std::size_t const j = bins[center].indices[iter_j];

                    if (i >= j) {
                        continue;
                    }

                    if (glm::distance2(points[i], points[j]) < dcut2) {
                        col_hash.feed(i, j);
                    }
                }
            }

            // between-case
            std::ptrdiff_t const deltas[] = {
                -1 - static_cast<std::ptrdiff_t>(nx),
                 0 - static_cast<std::ptrdiff_t>(nx),
                +1 - static_cast<std::ptrdiff_t>(nx),

                -1,
                +1,

                -1 + static_cast<std::ptrdiff_t>(nx),
                 0 + static_cast<std::ptrdiff_t>(nx),
                +1 + static_cast<std::ptrdiff_t>(nx),
            };

            for (std::ptrdiff_t delta : deltas) {
                std::size_t const other = static_cast<std::size_t>(static_cast<std::ptrdiff_t>(center) + delta);
                if (other >= bins.size()) {
                    continue;
                }

                for (std::size_t i : bins[center].indices) {
                    for (std::size_t j : bins[other].indices) {
                        if (i >= j) {
                            continue;
                        }

                        if (glm::distance2(points[i], points[j]) < dcut2) {
                            col_hash.feed(i, j);
                        }
                    }
                }
            }
        }
    }

    return col_hash;
}


CollisionHash spatial_hashing(std::vector<glm::dvec2> const& points, double radius)
{
    struct Bin
    {
        boost::container::small_vector<std::size_t, 16> indices;
    };

    double const dcut = 2 * radius;
    double const dcut2 = dcut * dcut;
    double const bin_freq = 1 / dcut;

    std::size_t const hash_map_size = 256;
    double const offset = 1 << 25;
    double const stride = static_cast<double>(hash_map_size);

    std::vector<Bin> hash_map(hash_map_size);

    for (std::size_t i = 0; i < points.size(); ++i) {
        glm::dvec2 const point = points[i];

        double const x = std::round(bin_freq * point.x) + offset;
        double const y = std::round(bin_freq * point.y) + offset;
        double const coord = x * stride + y;
        std::size_t const map_index = static_cast<std::size_t>(coord) % hash_map_size;

        hash_map[map_index].indices.push_back(i);
    }

    CollisionHash col_hash;

    for (std::size_t center = 0; center < hash_map_size; ++center) {

        // within-case
        std::size_t const n_center = hash_map[center].indices.size();

        for (std::size_t iter_i = 0; iter_i < n_center; ++iter_i) {
            for (std::size_t iter_j = 0; iter_j < n_center; ++iter_j) {
                std::size_t const i = hash_map[center].indices[iter_i];
                std::size_t const j = hash_map[center].indices[iter_j];

                if (i < j && glm::distance2(points[i], points[j]) < dcut2) {
                    col_hash.feed(i, j);
                }
            }
        }

        // between-case
        for (std::ptrdiff_t delta : {-1, +1}) {
            std::size_t const other = static_cast<std::size_t>(static_cast<std::ptrdiff_t>(center) + delta) % hash_map_size;

            for (std::size_t i : hash_map[center].indices) {
                for (std::size_t j : hash_map[other].indices) {
                    if (i < j && glm::distance2(points[i], points[j]) < dcut2) {
                        col_hash.feed(i, j);
                    }
                }
            }
        }
    }

    return col_hash;
}


int main()
{
    std::size_t const n_points = 100000;
    double const radius = 0.003;

    std::vector<glm::dvec2> points;

    std::mt19937_64 engine;
    std::uniform_real_distribution<double> coord{-1, 1};

    std::clog << "# Mean density = "
              << n_points * (M_PI * radius * radius) / (2 * 2)
              << '\n';

    for (std::size_t i = 0; i < n_points; ++i) {
        double const x = coord(engine);
        double const y = coord(engine);
        points.emplace_back(x, y);
    }

    auto const bench = [&](char const* name, auto func) {
        using Clock = std::chrono::steady_clock;
        using date::operator<<;

        Clock::time_point const start = Clock::now();
        CollisionHash const hash = func(points, radius);
        Clock::duration const elapsed = Clock::now() - start;

        std::cout << name
                  << ":\t"
                  << hash
                  << '\t'
                  << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
                  << '\n';
    };

    bench("Brute", brute_force);
    bench("Sweep", sweep_hash);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);
}
