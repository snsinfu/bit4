#ifndef INCLUDED_NEIGHBIR_SEARCHER_HPP
#define INCLUDED_NEIGHBIR_SEARCHER_HPP

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "geo.hpp"


namespace geo
{
    template<typename T>
    class neighbor_searcher
    {
        // Big prime numbers
        static constexpr std::size_t x_stride = 73856093;
        static constexpr std::size_t y_stride = 19349669;
        static constexpr std::size_t z_stride = 83492791;

    public:
        using scalar_type = T;
        using point_type = geo::point<scalar_type, 3>;

        neighbor_searcher(geo::index bin_count, scalar_type dcut)
            : bins_{bin_count}, dcut_{dcut}
        {
            std::size_t const index_deltas[3] = {
                bin_count - 1, bin_count, bin_count + 1
            };

            for (std::size_t dx : index_deltas) {
                for (std::size_t dy : index_deltas) {
                    for (std::size_t dz : index_deltas) {
                        hash_deltas_.push_back(hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas_.begin(), hash_deltas_.end());
            hash_deltas_.erase(std::unique(hash_deltas_.begin(), hash_deltas_.end()),
                               hash_deltas_.end());
        }

        void set_points(std::vector<point_type> const& points)
        {
            for (hash_bin& bin : bins_) {
                bin.members.clear();
            }

            scalar_type const bin_freq = 1 / dcut_;

            for (std::size_t index = 0; index < points.size(); ++index) {
                point_type const point = points[index];
                hash_bin& bin = bins_[locate_bin(point, bin_freq)];
                bin.members.push_back({index, point});
            }
        }

        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            scalar_type const dcut2 = dcut_ * dcut_;
            std::size_t const bin_count = bins_.size();

            for (std::size_t center = 0; center < bin_count; ++center) {
                auto const& center_members = bins_[center].members;

                for (std::size_t const delta : hash_deltas_) {
                    std::size_t const other = (center + delta) % bin_count;
                    auto const& other_members = bins_[other].members;

                    for (auto member_i : other_members) {
                        for (auto member_j : center_members) {
                            if (member_i.index >= member_j.index) {
                                continue;
                            }

                            if (squared_distance(member_i.point, member_j.point) > dcut2) {
                                continue;
                            }

                            *out++ = std::make_pair(member_i.index, member_j.index);
                        }
                    }
                }
            }
        }

    private:
        std::size_t locate_bin(point_type point, scalar_type freq) const
        {
            auto const to_size_t = [](scalar_type x) {
                // floating-point to unsigned integer conversion is costly
                return static_cast<std::size_t>(static_cast<std::ptrdiff_t>(x));
            };

            auto const x = to_size_t(std::nearbyint(freq * point[0]));
            auto const y = to_size_t(std::nearbyint(freq * point[1]));
            auto const z = to_size_t(std::nearbyint(freq * point[2]));

            return hash(x, y, z);
        }

        inline
        std::size_t hash(std::size_t x, std::size_t y, std::size_t z) const
        {
            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

    private:
        struct hash_bin
        {
            struct member
            {
                std::size_t index;
                point_type point;
            };
            std::vector<member> members;
        };

        std::vector<hash_bin> bins_;
        scalar_type dcut_;
        std::vector<std::size_t> hash_deltas_;
    };
}

#endif
