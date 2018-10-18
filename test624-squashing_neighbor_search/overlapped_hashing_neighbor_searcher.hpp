#ifndef INCLUDED_OVERLAPPED_HASHING_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_OVERLAPPED_HASHING_NEIGHBOR_SEARCHER_HPP

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"


namespace md
{
    class overlapped_hashing_neighbor_searcher
    {
        // Hash coefficients.
        static constexpr md::index x_stride = 3929498747;
        static constexpr md::index y_stride = 1008281837;
        static constexpr md::index z_stride = 1965268793;

    public:
        // Constructor takes the cutoff distance and the number of bins used for
        // hashing points.
        //
        // The optimal number of bins depends on the actual distribution of
        // points. A rule-of-thumb number for a dense system is the number of
        // points divided by 20.
        overlapped_hashing_neighbor_searcher(md::scalar dcut, md::index bins)
            : dcut_(dcut), bins_(bins)
        {
            md::index const coord_deltas[] = {
                bins - 1,
                bins,
                bins + 1
            };

            for (auto const dx : coord_deltas) {
                for (auto const dy : coord_deltas) {
                    for (auto const dz : coord_deltas) {
                        hash_deltas_.push_back(linear_hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas_.begin(), hash_deltas_.end());
            hash_deltas_.erase(
                std::unique(hash_deltas_.begin(), hash_deltas_.end()),
                hash_deltas_.end()
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
                md::index const center = locate_bin(pt);

                for (md::index delta : hash_deltas_) {
                    md::index const nearby = (center + delta) % bins_.size();
                    if (nearby > center) {
                        continue;
                    }

                    bins_[nearby].members.push_back({idx, pt});
                }
            }
        }

        // search searches for neighbor pairs in the hash table and outputs
        // index pairs to given output iterator. Each index pair (i,j) satisfies
        // i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            double const dcut2 = dcut_ * dcut_;

            for (hash_bin const& bin : bins_) {
                for (auto member_j : bin.members) {
                    for (auto member_i : bin.members) {
                        if (member_i.index == member_j.index) {
                            break;
                        }

                        if (md::squared_distance(member_i.point, member_j.point) > dcut2) {
                            continue;
                        }

                        *out++ = std::make_pair(
                            std::min(member_i.index, member_j.index),
                            std::max(member_i.index, member_j.index)
                        );
                    }
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
        std::vector<md::index> hash_deltas_;
    };
}

#endif
