#ifndef INCLUDED_HASHING_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_HASHING_NEIGHBOR_SEARCHER_HPP

#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"

#include <boost/container/static_vector.hpp>


namespace md
{
    namespace detail
    {
        inline bool is_prime(md::index num)
        {
            if (num % 2 == 0 || num % 3 == 0) {
                return false;
            }

            for (md::index iter = 6; iter < num / 2; iter++) {
                if (num % (iter - 1) == 0 || num % (iter + 1) == 0) {
                    return false;
                }
            }

            return true;
        }

        inline md::index next_prime(md::index num)
        {
            for (md::index iter = (num / 6 + 1) * 6; ; iter += 6) {
                if (detail::is_prime(iter - 1)) {
                    return iter - 1;
                }
                if (detail::is_prime(iter + 1)) {
                    return iter + 1;
                }
            }
        }
    }

    // hashing_neighbor_searcher is a data structure for searching pairs of
    // neighboring points in O(N) complexity.
    class hashing_neighbor_searcher
    {
        // Hash coefficients.
        static constexpr md::index x_stride = 3929498747;
        static constexpr md::index y_stride = 0;
        static constexpr md::index z_stride = 1008281837;

    public:
        // Constructor takes the cutoff distance and the number of bins used for
        // hashing points.
        //
        // The optimal number of bins depends on the actual distribution of
        // points. A rule-of-thumb number for a dense system is the number of
        // points divided by 20.
        hashing_neighbor_searcher(md::scalar dcut, md::index bins)
            // : dcut_(dcut), bins_(bins = detail::next_prime(bins))
            : dcut_(dcut), bins_(bins)
        {
            md::index const coord_deltas[] = {
                bins - 1,
                bins,
                bins + 1
            };
            std::vector<md::index> hash_deltas;

            for (auto const dx : coord_deltas) {
                for (auto const dy : coord_deltas) {
                    for (auto const dz : coord_deltas) {
                        hash_deltas.push_back(linear_hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas.begin(), hash_deltas.end());
            hash_deltas.erase(
                std::unique(hash_deltas.begin(), hash_deltas.end()),
                hash_deltas.end()
            );

            for (md::index center = 0; center < bins; center++) {
                auto& neighbors = bins_[center].neighbors;

                for (auto const delta : hash_deltas) {
                    auto const neighbor = (center + delta) % bins;

                    if (neighbor >= center) {
                        neighbors.push_back(neighbor);
                    }
                }
            }
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

            static bool first = true;
            if (first) {
                std::vector<int> counts;

                for (auto& bin : bins_) {
                    counts.push_back(int(bin.members.size()));
                }

                std::sort(counts.begin(), counts.end());

                std::cout << "Min: " << counts.front() << '\n';
                std::cout << "25%: " << counts.at(counts.size() / 4) << '\n';
                std::cout << "50%: " << counts.at(counts.size() / 2) << '\n';
                std::cout << "75%: " << counts.at(counts.size() * 3 / 4) << '\n';
                std::cout << "Max: " << counts.back() << '\n';

            }
            first = false;
        }

        // search searches for neighbor pairs in the hash table and outputs
        // index pairs to given output iterator. Each index pair (i,j) satisfies
        // i < j and no duplicate pairs are output.
        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            for (auto const& center_bin : bins_) {
                for (auto const neighbor : center_bin.neighbors) {
                    search_among(center_bin, bins_[neighbor], out);
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
            std::vector<md::index> neighbors;
        };

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
    };
}

#endif
