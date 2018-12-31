//------------------------------------------------------------------------------

#include <cmath>


namespace md
{
    struct vec
    {
        double x = 0;
        double y = 0;
        double z = 0;

        double norm() const;
    };

    md::vec operator-(md::vec const& v1, md::vec const& v2);
}

inline
double md::vec::norm() const
{
    return std::sqrt(x * x + y * y + z * z);
}

inline
md::vec md::operator-(md::vec const& v1, md::vec const& v2)
{
    return md::vec{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}


//------------------------------------------------------------------------------

namespace md
{
    struct box
    {
        md::vec lower_bound;
        md::vec upper_bound;
    };
}


//------------------------------------------------------------------------------

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
#include <vector>


namespace md
{
    class periodic_neighbor_searcher
    {
    public:
        periodic_neighbor_searcher(md::box cell, double dcut);

        // set_points sets points in the periodic box.
        void set_points(std::vector<md::vec> const& points);

        // search searches for neighbor pairs in the system.
        template<typename Callback>
        void search(Callback callback) const;

    private:
        struct hash_bucket
        {
            struct member
            {
                md::vec point;
                int     index;
            };
            std::vector<member>        members;
            std::vector<std::uint32_t> adjacent_buckets;
        };

        md::box                  cell_;
        double                   dcut_;
        std::vector<hash_bucket> buckets_;
    };
}

inline
md::periodic_neighbor_searcher::periodic_neighbor_searcher(
    md::box cell, double dcut
)
    : cell_{cell}, dcut_{dcut}
{
}

inline
void md::periodic_neighbor_searcher::set_points(
    std::vector<md::vec> const& points
)
{
    constexpr double coord_offset = 1e10;
    constexpr double bucket_density = 5.5;
    constexpr std::uint32_t prime_x = 1751364113;
    constexpr std::uint32_t prime_y = 1921636979;
    constexpr std::uint32_t prime_z = 1883880683;
    auto const bucket_count = std::uint64_t(points.size() / bucket_density);

    auto const span = cell_.upper_bound - cell_.lower_bound;
    auto const grid_x = std::uint32_t(std::ceil(span.x / dcut_));
    auto const grid_y = std::uint32_t(std::ceil(span.y / dcut_));
    auto const grid_z = std::uint32_t(std::ceil(span.z / dcut_));

    auto const discretize = [=](double x) {
        return std::uint32_t(std::floor(coord_offset + x / dcut_));
    };

    buckets_.clear();
    buckets_.resize(std::size_t(bucket_count));

    // Precompute adjacent buckets
    std::uint32_t deltas_x[] = {grid_x - 1, 0, 1};
    std::uint32_t deltas_y[] = {grid_y - 1, 0, 1};
    std::uint32_t deltas_z[] = {grid_z - 1, 0, 1};

    for (std::uint32_t bucket_idx = 0; bucket_idx < bucket_count; bucket_idx++) {
        std::set<std::uint32_t> adjacent_buckets;

        for (auto const dx : deltas_x)
        for (auto const dy : deltas_y)
        for (auto const dz : deltas_z) {
            std::uint64_t hash_delta = 0;
            hash_delta += prime_x * dx;
            hash_delta += prime_y * dy;
            hash_delta += prime_z * dz;

            auto const adjacent_idx = std::uint32_t(
                (bucket_idx + hash_delta) % bucket_count
            );
            if (adjacent_idx >= bucket_idx) {
                adjacent_buckets.insert(adjacent_idx);
            }
        }

        buckets_[bucket_idx].adjacent_buckets.assign(
            adjacent_buckets.begin(),
            adjacent_buckets.end()
        );
    }

    // Assign points to buckets
    for (std::size_t idx = 0; idx < points.size(); idx++) {
        auto const r = points[idx] - cell_.lower_bound;
        auto const x = discretize(r.x) % grid_x;
        auto const y = discretize(r.y) % grid_y;
        auto const z = discretize(r.z) % grid_z;

        std::uint64_t hash = 0;
        hash += prime_x * x;
        hash += prime_y * y;
        hash += prime_z * z;
        hash %= bucket_count;

        buckets_[hash].members.push_back(hash_bucket::member{
            .point = points[idx],
            .index = static_cast<int>(idx)
        });
    }
}

template<typename Callback>
inline
void md::periodic_neighbor_searcher::search(Callback callback) const
{
    auto const span = cell_.upper_bound - cell_.lower_bound;

    for (auto const& bucket : buckets_) {
        for (auto const adjacent_index : bucket.adjacent_buckets) {
            auto const& adjacent_bucket = buckets_[adjacent_index];

            for (auto const m1 : bucket.members)
            for (auto const m2 : adjacent_bucket.members) {
                if (m1.index == m2.index) {
                    // Avoid double counting
                    break;
                }

                auto r = m1.point - m2.point;
                r.x = std::remainder(r.x, span.x);
                r.y = std::remainder(r.y, span.y);
                r.z = std::remainder(r.z, span.z);

                if (r.norm() < dcut_) {
                    auto const i = std::min(m1.index, m2.index);
                    auto const j = std::max(m1.index, m2.index);
                    callback(i, j);
                }
            }
        }
    }
}


//------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <utility>


namespace
{
    std::vector<md::vec> generate_points(
        md::box domain, int count, std::mt19937& random
    )
    {
        std::vector<md::vec> points;
        std::uniform_real_distribution<double> uniform{0, 1};

        std::generate_n(std::back_inserter(points), count, [&] {
            md::vec pt = domain.lower_bound;
            md::vec span = domain.upper_bound - domain.lower_bound;
            pt.x += uniform(random) * span.x;
            pt.y += uniform(random) * span.y;
            pt.z += 0.5 * span.z;
            //pt.z += uniform(random) * span.z;
            return pt;
        });

        return points;
    }
}


int main()
{
    md::box domain{
        .lower_bound = {0, 0, 0},
        .upper_bound = {1, 1, 1}
    };
    md::periodic_neighbor_searcher searcher{domain, 0.1};

    std::mt19937 random;
    std::vector<md::vec> const points = generate_points(domain, 25, random);
    searcher.set_points(points);

    std::set<std::pair<int, int>> expected;
    for (std::size_t j = 0; j < points.size(); j++) {
        for (std::size_t i = 0; i < j; i++) {
            auto r = points[i] - points[j];
            r.x = std::remainder(r.x, 1);
            r.y = std::remainder(r.y, 1);
            r.z = std::remainder(r.z, 1);

            if (r.norm() < 0.1) {
                expected.insert(std::make_pair(i, j));
            }
        }
    }

    std::set<std::pair<int, int>> actual;
    searcher.search([&](int i, int j) {
        actual.insert(std::make_pair(i, j));
        std::cout << i << '\t' << j << '\n';
    });

    if (actual == expected) {
        std::cout << "OK\n";
    } else {
        std::cout << "BAD\n";
    }
}
