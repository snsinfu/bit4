#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include <md.hpp>

#include "box.hpp"


// Bucket for spatial hash table.
struct spatial_bucket
{
    struct member
    {
        md::index index;
        md::point point;
    };

    // Points in the bucket.
    std::vector<member> members;

    // Indices of the buckets spatially adjoining to this bucket.
    std::vector<md::index> complete_neighbors;

    // Subset of complete_neighbors where bidirectional links in the adjoining
    // graph are reduced to a unidirectional one.
    std::vector<md::index> directed_neighbors;
};


// Internal neighbor searcher implementation for generic grid.
template<typename Grid>
class basic_neighbor_searcher
{
public:
    basic_neighbor_searcher(Grid grid, md::scalar dcut)
        : grid_{grid}, dcut_{dcut}
    {
    }

    // Clears grid buckets and assigns given points there.
    void set_points(md::array_view<md::point const> points)
    {
        for (auto& bucket : grid_.buckets) {
            bucket.members.clear();
        }

        for (md::index index = 0; index < points.size(); index++) {
            auto const point = points[index];
            auto const bucket_index = grid_.locate_bucket(point);
            auto& bucket = grid_.buckets[bucket_index];
            bucket.members.push_back({ index, point });
        }
    }

    // Outputs pairs of indices of neighboring points to given output iterator.
    // Each index pair (i,j) satisfies i < j. No duplicates are reported.
    template<typename OutputIterator>
    void search(OutputIterator out) const
    {
        for (auto const& bucket : grid_.buckets) {
            for (auto const neighbor_index : bucket.directed_neighbors) {
                search_among(bucket, grid_.buckets[neighbor_index], out);
            }
        }
    }

    // Outputs the indices of points neighboring to given point.
    template<typename OutputIterator>
    void query(md::point point, OutputIterator out) const
    {
        auto const bucket_index = grid_.locate_bucket(point);
        auto const& bucket = grid_.buckets[bucket_index];
        for (auto const neighbor_index : bucket.complete_neighbors) {
            search_among(bucket, grid_.buckets[neighbor_index], out);
        }
    }

private:
    // Searches a pair of buckets for neighboring pairs of points.
    template<typename OutputIterator>
    inline void search_among(
        spatial_bucket const& bucket_a,
        spatial_bucket const& bucket_b,
        OutputIterator& out
    ) const
    {
        auto const dcut2 = dcut_ * dcut_;

        for (auto const member_j : bucket_b.members) {
            for (auto const member_i : bucket_a.members) {
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
    Grid grid_;
    md::scalar dcut_;
};


template<typename Box>
struct search_grid
{
    // Exposition only. Not implemented.
    search_grid(Box box, md::scalar spacing, md::index count);
};


// Grid implementation for open_box. Since infinite grid cannot be represented
// in memory, so this implementation uses a hash function with finite modulus.
template<>
struct search_grid<open_box>
{
    md::scalar                  spacing;
    md::linear_hash             hash;
    std::vector<spatial_bucket> buckets;

    search_grid(open_box box, md::scalar spacing)
        : spacing{spacing}
    {
        init_hash(box);
        init_buckets();
    }

    inline std::uint32_t locate_bucket(md::point pt) const
    {
        // Negative coordinate value causes discontinuous jumps in hash value
        // which breaks our search algorithm. Avoid that by offsetting. The
        // offset should be chosen to not overwhelm the coordinate values.
        constexpr md::scalar offset = 1L << 20;

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

private:
    void init_hash(open_box box)
    {
        // This simple heuristic gives surprisingly good performance.
        hash.modulus = md::linear_hash::uint(box.particle_count * 2 / 11);
        hash.modulus |= 1;
    }

    void init_buckets()
    {
        // Compute neighbor graph of the spatial cells.
        std::set<std::uint32_t> hash_deltas;

        std::uint32_t const coord_deltas[] = {
            hash.modulus - 1,
            hash.modulus,
            hash.modulus + 1
        };

        for (auto const dx : coord_deltas) {
            for (auto const dy : coord_deltas) {
                for (auto const dz : coord_deltas) {
                    hash_deltas.insert(hash(dx, dy, dz));
                }
            }
        }

        for (std::uint32_t center = 0; center < hash.modulus; center++) {
            auto& directed_neighbors = buckets[center].directed_neighbors;
            auto& complete_neighbors = buckets[center].complete_neighbors;

            for (auto const delta : hash_deltas) {
                auto const neighbor = (center + delta) % hash.modulus;

                // Leverage symmetry to reduce search space.
                if (neighbor >= center) {
                    directed_neighbors.push_back(neighbor);
                }
                complete_neighbors.push_back(neighbor);
            }
        }
    }
};


// Generic neighbor searcher API.
template<typename Box>
class neighbor_searcher
{
    using grid_type = search_grid<Box>;

public:
    neighbor_searcher(Box box, md::scalar dcut)
        : searcher_{grid_type{box, dcut}, dcut}
    {
    }

    // Clears grid buckets and assigns given points there.
    void set_points(md::array_view<md::point const> points)
    {
        searcher_.set_points(points);
    }

    // Outputs pairs of indices of neighboring points to given output iterator.
    // Each index pair (i,j) satisfies i < j. No duplicates are reported.
    template<typename OutputIterator>
    void search(OutputIterator out) const
    {
        searcher_.search(out);
    }

    // Outputs the indices of points neighboring to given point.
    template<typename OutputIterator>
    void query(md::point point, OutputIterator out) const
    {
        searcher_.query(point, out);
    }

private:
    basic_neighbor_searcher<grid_type> searcher_;
};
