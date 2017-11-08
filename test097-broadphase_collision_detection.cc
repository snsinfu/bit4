// Broad phase algorithm for detecting collisions among uniform spheres

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <dak.hpp>

namespace broad
{
    using distance = dak::scalar<double, dak::dim::length>;
    using point = dak::point<double, dak::dim::length, 3>;
    using vector = dak::vector<double, dak::dim::length, 3>;

    /*
     * Hash for point objects,
     */
    class linear_spatial_hasher
    {
      public:
        linear_spatial_hasher(broad::distance grid_interval, std::uint32_t modulo)
            : grid_interval_{grid_interval}
            , modulo_{modulo}
        {
        }

        std::uint32_t hash(broad::point const& point) const
        {
            std::uint32_t hash = 0;
            for (unsigned axis = 0; axis < broad::point::dimension; ++axis) {
                hash += discretize(point[axis]) * coefficients_[axis];
            }
            return hash % modulo_;
        }

        std::uint32_t modulo() const noexcept
        {
            return modulo_;
        }

      private:
        std::uint32_t discretize(broad::distance coord) const
        {
            return to_uint(std::nearbyint(coord / grid_interval_));
        }

        std::uint32_t to_uint(double number) const
        {
            return static_cast<std::uint32_t>(static_cast<std::int32_t>(number));
        }

      private:
        broad::distance              grid_interval_;
        std::uint32_t                modulo_;
        std::array<std::uint32_t, 3> coefficients_{{73856093, 19349669, 83492791}};
    };

    class particles_collision_search
    {
      public:
        explicit particles_collision_search(linear_spatial_hasher hasher)
            : hasher_{hasher}
            , buckets_(hasher.modulo())
        {
        }

        void update(std::vector<broad::point> positions)
        {
            positions_ = std::move(positions);
            rebuild_buckets();
        }

      private:
        struct grid_bucket
        {
            std::vector<std::uint32_t> indices;
        };

        void rebuild_buckets()
        {
            clear_buckets();
            for (std::uint32_t index = 0; index < positions_.size(); ++index) {
                std::uint32_t const bucket_index = hasher_.hash(positions_[index]);
                buckets_[bucket_index].indices.push_back(index);
            }
        }

        void clear_buckets()
        {
            for (grid_bucket& bucket : buckets_) {
                bucket.indices.clear();
            }
        }

      private:
        linear_spatial_hasher hasher_;
        std::vector<broad::point> positions_;
        std::vector<grid_bucket> buckets_;
    };
}

int main()
{
    broad::linear_spatial_hasher hasher{broad::distance{0.1}, 100};
    broad::particles_collision_search search{hasher};

    std::vector<broad::point> points;
    std::mt19937_64 engine;
    std::uniform_real_distribution<double> coord{-1, 1};
    for (int i = 0; i < 1000; ++i) {
        points.emplace_back(
            coord(engine),
            coord(engine),
            coord(engine)
        );
    }

    search.update(points);
}
