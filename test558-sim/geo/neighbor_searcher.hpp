// Efficient neighbor search plugin for geo.hpp.

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_NEIGHBIR_SEARCHER_HPP
#define INCLUDED_NEIGHBIR_SEARCHER_HPP

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "geo.hpp"


namespace geo
{
    // neighbor_searcher is a data structure for efficiently searching pairs of points that are
    // within a fixed "cutoff" distance in the three-dimensional space.
    template<typename T>
    class neighbor_searcher
    {
    public:
        using scalar_type = T;
        using point_type = geo::point<scalar_type, 3>;
        using index_type = std::size_t;

        // Constructor takes the cutoff distance and the number of bins used for hashing points.
        neighbor_searcher(scalar_type dcut, index_type bins)
            : dcut_(dcut), bins_(bins)
        {
            // Pre-compute the hash index differences between adjacent bins. We use a linear hash
            // function so the differences are the same (i.e., reusable) for all bins.

            index_type const index_deltas[] = {bins - 1, bins, bins + 1};

            for (auto dx : index_deltas) {
                for (auto dy : index_deltas) {
                    for (auto dz : index_deltas) {
                        hash_deltas_.push_back(hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas_.begin(), hash_deltas_.end());
            hash_deltas_.erase(
                std::unique(hash_deltas_.begin(), hash_deltas_.end()),
                hash_deltas_.end()
            );
        }

        // set_points clears internal hash table and assigns given points to the table.
        void set_points(std::vector<point_type> const& points)
        {
            for (hash_bin& bin : bins_) {
                bin.members.clear();
            }

            for (index_type index = 0; index < points.size(); ++index) {
                point_type const point = points[index];
                hash_bin& bin = bins_[locate_bin(point)];
                bin.members.push_back({index, point});
            }
        }

        // search searches for neighbor pairs in the hash table and outputs index pairs to given
        // output iterator. Each index pair (i,j) satisfies i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            index_type const bin_count = bins_.size();

            for (index_type center = 0; center < bin_count; ++center) {
                hash_bin const& center_bin = bins_[center];

                for (index_type const delta : hash_deltas_) {
                    hash_bin const& other_bin = bins_[(center + delta) % bin_count];

                    search_among(center_bin, other_bin, out);
                }
            }
        }

    private:
        struct hash_bin
        {
            struct member
            {
                index_type index;
                point_type point;
            };
            std::vector<member> members;
        };

        // Prime hash coefficients.
        static constexpr index_type x_stride = 73856093;
        static constexpr index_type y_stride = 19349669;
        static constexpr index_type z_stride = 83492791;

        // search_among searches for pairs of neighbor points in given bins. It outputs index pairs
        // to out. Each index pair (i,j) satisfies i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        inline void search_among(
            hash_bin const& center_bin,
            hash_bin const& other_bin,
            OutputIterator& out
        ) const
        {
            scalar_type const dcut2 = dcut_ * dcut_;

            for (auto member_i : other_bin.members) {
                for (auto member_j : center_bin.members) {
                    if (member_i.index >= member_j.index) {
                        continue;
                    }

                    if (geo::squared_distance(member_i.point, member_j.point) > dcut2) {
                        continue;
                    }

                    *out++ = std::make_pair(member_i.index, member_j.index);
                }
            }
        }

        // locate_bin returns the hash index for a point.
        inline index_type locate_bin(point_type point) const
        {
            // Floating-point to *unsigned* integer conversion is costly.
            auto const to_index_type = [](scalar_type x) {
                return static_cast<index_type>(static_cast<std::ptrdiff_t>(x));
            };

            auto const freq = 1 / dcut_;
            auto const x = to_index_type(std::nearbyint(freq * point[0]));
            auto const y = to_index_type(std::nearbyint(freq * point[1]));
            auto const z = to_index_type(std::nearbyint(freq * point[2]));

            return hash(x, y, z);
        }

        // hash returns the index into the hash table bins_ for given three dimensional bin index
        // (x,y,z).
        inline index_type hash(index_type x, index_type y, index_type z) const
        {
            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

    private:
        scalar_type dcut_;
        std::vector<hash_bin> bins_;
        std::vector<index_type> hash_deltas_;
    };
}

#endif
