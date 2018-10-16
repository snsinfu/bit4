#ifndef INCLUDED_HASHING_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_HASHING_NEIGHBOR_SEARCHER_HPP

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"


namespace md
{
    // hashing_neighbor_searcher is a data structure for searching pairs of
    // neighboring points in O(N) complexity.
    class hashing_neighbor_searcher
    {
    public:
        // Prime coefficients used for coordinate hashing.
        static constexpr md::index x_stride = 73856093;
        static constexpr md::index y_stride = 19349669;
        static constexpr md::index z_stride = 83492791;

        // Constructor takes the cutoff distance and the number of bins used for
        // hashing points.
        //
        // The optimal number of bins depends on the actual distribution of
        // points. A rule-of-thumb number for a dense system is the number of
        // points divided by 20.
        hashing_neighbor_searcher(md::scalar dcut, md::index bins)
            : dcut_(dcut), bins_(bins)
        {
            // Pre-compute the hash index differences between adjacent bins. We
            // use a linear hash function so the differences are the same (i.e.,
            // reusable) for all bins.

            md::index const index_deltas[] = {bins - 1, bins, bins + 1};

            for (auto dx : index_deltas) {
                for (auto dy : index_deltas) {
                    for (auto dz : index_deltas) {
                        nearby_deltas_.push_back(linear_hash(dx, dy, dz));
                    }
                }
            }

            std::sort(nearby_deltas_.begin(), nearby_deltas_.end());

            nearby_deltas_.erase(
                std::unique(nearby_deltas_.begin(), nearby_deltas_.end()),
                nearby_deltas_.end()
            );
        }

        // set_points clears internal hash table and assigns given points to the
        // hash table.
        void set_points(md::array_view<md::point const> points)
        {
            for (hash_bin& bin : bins_) {
                bin.members.clear();
            }

            for (md::index idx = 0; idx < points.size(); ++idx) {
                md::point const pt = points[idx];
                hash_bin& bin = bins_[locate_bin(pt)];
                bin.members.push_back({idx, pt});
            }
        }

        // search searches for neighbor pairs in the hash table and outputs
        // index pairs to given output iterator. Each index pair (i,j) satisfies
        // i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            md::index const bin_count = bins_.size();

            for (md::index center = 0; center < bin_count; center++) {
                for (md::index const delta : nearby_deltas_) {
                    auto nearby = center + delta;

                    if (nearby >= bin_count) {
                        nearby -= bin_count;
                    }

                    hash_bin const& center_bin = bins_[center];
                    hash_bin const& nearby_bin = bins_[nearby];

                    search_among(center_bin, nearby_bin, out);
                }
            }
        }

    private:
        struct hash_bin
        {
            struct member
            {
                md::index index;
                md::point point;
            };
            std::vector<member> members;
        };

        // search_among searches for pairs of neighbor points in given bins. It
        // outputs index pairs to out. Each index pair (i,j) satisfies i < j
        // and no duplicate pairs are output.
        template<typename OutputIterator>
        inline void search_among(
            hash_bin const& bin_a,
            hash_bin const& bin_b,
            OutputIterator& out
        ) const
        {
            auto const dcut2 = dcut_ * dcut_;

            for (auto member_j : bin_b.members) {
                for (auto member_i : bin_a.members) {
                    if (member_i.index >= member_j.index) {
                        break;
                    }

                    if (md::squared_distance(member_i.point, member_j.point) > dcut2) {
                        continue;
                    }

                    *out++ = std::make_pair(member_i.index, member_j.index);
                }
            }
        }

        // locate_bin returns the hash index for a point.
        inline md::index locate_bin(md::point pt) const
        {
            // Wraparound of negative value breaks the linearity of our hash
            // function. Avoid that by offsetting.
            auto const offset = md::scalar(1L << 30);

            auto const freq = 1 / dcut_;
            auto const x = md::index(offset + freq * pt.x);
            auto const y = md::index(offset + freq * pt.y);
            auto const z = md::index(offset + freq * pt.z);

            return linear_hash(x, y, z);
        }

        // linear_hash returns the index into the hash table bins_ for given
        // three dimensional bin index (x,y,z).
        inline md::index linear_hash(md::index x, md::index y, md::index z) const
        {
            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

    private:
        md::scalar dcut_;
        std::vector<hash_bin> bins_;
        std::vector<md::index> nearby_deltas_;
    };
}

#endif
