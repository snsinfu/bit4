#ifndef INCLUDED_SQUASHING_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_SQUASHING_NEIGHBOR_SEARCHER_HPP

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"


namespace md
{
    class squashing_neighbor_searcher
    {
        static constexpr md::index x_stride = 3929498747;
        static constexpr md::index y_stride = 2035534877;
        static constexpr md::index z_stride = 1008281837;

    public:
        squashing_neighbor_searcher(md::scalar dcut, md::index buckets)
            : dcut_(dcut), buckets_(((buckets += 1) /= 2) *= 2)
        {
            assert(buckets >= 3);

            // Precompute neighborhood of each bucket.
            md::index const coord_deltas[] = {
                0, 1, 2
            };
            std::vector<md::index> hash_deltas;

            for (auto const dx : coord_deltas) {
                for (auto const dy : coord_deltas) {
                    for (auto const dz : coord_deltas) {
                        hash_deltas.push_back(hash(dx, dy, dz));
                    }
                }
            }

            std::sort(hash_deltas.begin(), hash_deltas.end());
            hash_deltas.erase(
                std::unique(hash_deltas.begin(), hash_deltas.end()),
                hash_deltas.end()
            );

            for (md::index center = 0; center < buckets; center++) {
                auto& neighbors = buckets_[center].neighbors;

                for (md::index const delta : hash_deltas) {
                    md::index const neighbor = (center + delta) % buckets;

                    if (neighbor < center) {
                        neighbors.push_back(neighbor);
                    }
                }

                std::sort(neighbors.begin(), neighbors.end());
                neighbors.erase(
                    std::unique(neighbors.begin(), neighbors.end()),
                    neighbors.end()
                );
            }
        }

        void set_points(md::array_view<md::point const> points)
        {
            for (hash_bucket& bucket : buckets_) {
                bucket.members.clear();
            }

            for (md::index idx = 0; idx < points.size(); ++idx) {
                md::point const pt = points[idx];
                hash_bucket& bucket = buckets_[locate_bucket(pt)];
                bucket.members.push_back({idx, pt});
            }

            static bool first = true;
            if (first) {
                std::vector<int> counts;

                for (hash_bucket const& bucket : buckets_) {
                    counts.push_back(int(bucket.members.size()));
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

        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            for (auto const& center_bucket : buckets_) {
                for (auto const neighbor_idx : center_bucket.neighbors) {
                    search_among(center_bucket, buckets_[neighbor_idx], out);
                }
            }
        }

    private:
        struct hash_bucket
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
            hash_bucket const& bucket_a,
            hash_bucket const& bucket_b,
            OutputIterator& out
        ) const
        {
            md::scalar const dcut2 = dcut_ * dcut_;

            for (auto member_j : bucket_b.members) {
                for (auto member_i : bucket_a.members) {
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

        // locate_bucket returns the hash index for a point.
        inline md::index locate_bucket(md::point pt) const
        {
            // Wraparound of negative value breaks the linearity of our hash
            // function. Avoid that by offsetting.
            auto const offset = md::scalar(1L << 30);

            auto const freq = 1 / dcut_;
            auto const x = md::index(offset + freq * pt.x);
            auto const y = md::index(offset + freq * pt.y);
            auto const z = md::index(offset + freq * pt.z);

            return hash(x, y, z);
        }

        inline md::index hash(md::index x, md::index y, md::index z) const
        {
            return ((x / 2) * x_stride + (y / 2) * y_stride + (z / 2) * z_stride) % buckets_.size();
        }

    private:
        md::scalar dcut_;
        std::vector<hash_bucket> buckets_;
    };
}

#endif
