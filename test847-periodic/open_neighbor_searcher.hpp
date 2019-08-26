#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include <md.hpp>

#include "box.hpp"


// hash_bucket is a bucket in a spatial hash table.
struct hash_bucket
{
    struct member
    {
        md::index index;
        md::point point;
    };
    std::vector<member> members;
    std::vector<md::index> directed_neighbors;
    std::vector<md::index> complete_neighbors;
};


struct grid_concept
{
    std::vector<hash_bucket> buckets;
    std::size_t locate_bucket(md::point point) const;
};


template<typename Grid>
class basic_neighbor_searcher
{
public:
    basic_neighbor_searcher(md::scalar dcut, Grid grid)
        : dcut_{dcut}, grid_{grid}
    {
    }

    // set_points assigns given points to hash table for subsequent queries.
    void set_points(md::array_view<md::point const> points)
    {
        for (hash_bucket& bucket : grid_.buckets) {
            bucket.members.clear();
        }

        for (md::index index = 0; index < points.size(); index++) {
            md::point const point = points[index];
            hash_bucket& bucket = grid_.buckets[grid_.locate_bucket(point)];
            bucket.members.push_back({ index, point });
        }
    }

    // search outputs pairs of indices of neighboring points to given output
    // iterator. Each index pair (i,j) satisfies i < j. No duplicates are
    // reported.
    template<typename OutputIterator>
    void search(OutputIterator out) const
    {
        for (hash_bucket const& center : grid_.buckets) {
            for (md::index const neighbor : center.directed_neighbors) {
                search_among(center, grid_.buckets[neighbor], out);
            }
        }
    }

    // query outputs the indices of points neighboring to given point.
    template<typename OutputIterator>
    void query(md::point point, OutputIterator out) const
    {
        hash_bucket const& center = grid_.buckets[grid_.locate_bucket(point)];
        for (md::index const neighbor : center.complete_neighbors) {
            search_among(center, grid_.buckets[neighbor], out);
        }
    }

private:
    // search_among searches a pair of buckets for all neighboring pairs.
    template<typename OutputIterator>
    inline void search_among(
        hash_bucket const& bucket_a,
        hash_bucket const& bucket_b,
        OutputIterator& out
    ) const
    {
        md::scalar const dcut2 = dcut_ * dcut_;

        for (typename hash_bucket::member member_j : bucket_b.members) {
            for (typename hash_bucket::member member_i : bucket_a.members) {
                if (member_i.index == member_j.index) {
                    // Avoid double counting when bucket_a == bucket_b.
                    break;
                }

                if (grid_.squared_distance(member_i.point, member_j.point) > dcut2) {
                    continue;
                }

                *out++ = std::make_pair(
                    std::min(member_i.index, member_j.index),
                    std::max(member_i.index, member_j.index)
                );
            }
        }
    }

private:
    md::scalar dcut_;
    Grid grid_;
};


struct open_grid
{
    md::scalar               spacing;
    md::linear_hash          hash;
    std::vector<hash_bucket> buckets;

    open_grid(md::scalar spacing, md::linear_hash hash)
        : spacing{spacing}, hash{hash}, buckets(hash.modulus)
    {
        std::set<std::uint32_t> hash_deltas;

        // Pre-compute neighborhood of each bucket for faster query.
        std::uint32_t const coord_deltas[] = {
            hash.modulus - 1,
            hash.modulus,
            hash.modulus + 1
        };

        for (std::uint32_t const dx : coord_deltas) {
            for (std::uint32_t const dy : coord_deltas) {
                for (std::uint32_t const dz : coord_deltas) {
                    hash_deltas.insert(hash(dx, dy, dz));
                }
            }
        }

        for (std::uint32_t center = 0; center < hash.modulus; center++) {
            std::vector<md::index>& directed_neighbors = buckets[center].directed_neighbors;
            std::vector<md::index>& complete_neighbors = buckets[center].complete_neighbors;

            for (std::uint32_t const delta : hash_deltas) {
                std::uint32_t const neighbor = (center + delta) % hash.modulus;

                // Leverage symmetry to reduce search space.
                if (neighbor >= center) {
                    directed_neighbors.push_back(neighbor);
                }
                complete_neighbors.push_back(neighbor);
            }
        }
    }

    inline std::uint32_t locate_bucket(md::point pt) const
    {
        // Negative coordinate value causes discontinuous jumps in hash
        // value which breaks our search algorithm. Avoid that by
        // offsetting.
        constexpr md::scalar offset = 1L << 16;

        md::scalar const freq = 1 / spacing;
        auto const x = std::uint32_t(offset + freq * pt.x);
        auto const y = std::uint32_t(offset + freq * pt.y);
        auto const z = std::uint32_t(offset + freq * pt.z);

        return hash(x, y, z);
    }

    inline md::scalar squared_distance(md::point p1, md::point p2) const
    {
        return md::squared_distance(p1, p2);
    }
};


// Neighbor searcher optimized for open system.
class open_neighbor_searcher : public basic_neighbor_searcher<open_grid>
{
public:
    open_neighbor_searcher(md::scalar dcut, md::linear_hash hash)
        : basic_neighbor_searcher<open_grid>{dcut, {dcut, hash}}
    {
    }
};


struct bin_layout
{
    md::scalar step;
    std::uint32_t count;
};


struct xyz_periodic_grid
{
    md::scalar spacing;
    periodic_box box;
    bin_layout x_bins;
    bin_layout y_bins;
    bin_layout z_bins;
    std::vector<hash_bucket> buckets;

    xyz_periodic_grid(md::scalar min_spacing, periodic_box box)
    {
    }

    inline std::uint32_t locate_bucket(md::point pt) const
    {
    }

    inline md::scalar squared_distance(md::point p1, md::point p2) const
    {
        return box.shortest_displacement(p1, p2).squared_norm();
    }
};


// Neighbor searcher optimized for periodic box.
class periodic_neighbor_searcher : public basic_neighbor_searcher<xyz_periodic_grid>
{
public:
    periodic_neighbor_searcher(md::scalar dcut, periodic_box box)
        : basic_neighbor_searcher<xyz_periodic_grid>{dcut, {dcut, box}}
    {
    }
};
