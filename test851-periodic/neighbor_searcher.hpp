#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include <md.hpp>

#include "box.hpp"


namespace detail
{
    using std::int32_t;
    using std::uint32_t;

    // Computes non-negative floating-point remainder of x/m. The result r
    // satisfies 0 <= r < m. Use this function to map a point to its canonical
    // image in a periodic system.
    inline md::scalar floor_mod(md::scalar x, md::scalar m)
    {
        return x - std::floor(x * (1 / m)) * m;
    }

    // Computes zero-centered floating-point remainder of x/m. The result r
    // satisfies -m/2 <= r <= m/2. Use this function to compute the distance
    // between nearest images of points in a periodic system.
    // inline md::scalar round_mod(md::scalar x, md::scalar m)
    // {
    //     return x - std::nearbyint(x * (1 / m)) * m;
    // }

    // Returns the integral part of x as uint32_t.
    inline uint32_t trunc_uint(md::scalar x)
    {
        // Converting floating-point number to int32_t is much faster than to
        // uint32_t on x86.
        return static_cast<uint32_t>(static_cast<int32_t>(x));
    }

    // Rounds x to the nearest uint32_t value.
    inline uint32_t round_uint(md::scalar x)
    {
        return trunc_uint(std::nearbyint(x));
    }

    // Computes (x + y) mod m assuming both x and y are less than m.
    inline uint32_t add_mod(uint32_t x, uint32_t y, uint32_t m)
    {
        auto sum = x + y;
        if (sum >= m) {
            sum -= m;
        }
        return sum;
    }

    // A bin_layout defines 1-dimensional uniform bins.
    struct bin_layout
    {
        md::scalar step;
        uint32_t count;
    };

    // Creates a bin_layout that splits given span into uniform bins.
    inline bin_layout define_bins(md::scalar span, md::scalar min_step)
    {
        bin_layout bins;
        bins.count = std::max(trunc_uint(span / min_step), uint32_t(1));
        bins.step = span / md::scalar(bins.count);
        return bins;
    }

    // Computes the index of the bin in which given coordinate value falls.
    inline uint32_t locate_bin(const bin_layout& bins, md::scalar coord)
    {
        const auto span = bins.step * md::scalar(bins.count);
        const auto image = floor_mod(coord, span);
        const auto index = trunc_uint(image * (1 / bins.step));
        return index < bins.count ? index : bins.count - 1;
    }

    // Sort and remove duplicates in a container.
    template<typename Container>
    void sort_unique(Container& cont)
    {
        std::sort(cont.begin(), cont.end());
        cont.erase(std::unique(cont.begin(), cont.end()), cont.end());
    }
}


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


// Grid abstracts away how points should be grouped into spatial neighbors in
// given box.
template<typename Box>
struct search_grid
{
    // Here shown is not an implementation but the interface definition.

    // Groups of points.
    std::vector<spatial_bucket> buckets;

    // Constructor builds the buckets.
    search_grid(Box box, md::scalar spacing);

    // Returns the index of the bucket the given point would be assigned to.
    std::size_t locate_bucket(md::point point) const;

    // Returns the squared distance between points in the system.
    md::scalar squared_distance(md::point p1, md::point p2) const;
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


// Helper class for generating x/y/z_bins for given box and spacing.
template<typename Box>
struct basic_binner
{
};


// Generic implementation of search_grid for bin-based grid construction.
template<typename Box>
struct binned_search_grid
{
    Box                         box;
    detail::bin_layout          x_bins;
    detail::bin_layout          y_bins;
    detail::bin_layout          z_bins;
    std::vector<spatial_bucket> buckets;

    binned_search_grid(Box box, md::scalar spacing)
        : box{box}
    {
        init_bins(spacing);
        init_buckets();
    }

    inline std::size_t locate_bucket(md::point pt) const
    {
        const auto x = detail::locate_bin(x_bins, pt.x);
        const auto y = detail::locate_bin(y_bins, pt.y);
        const auto z = detail::locate_bin(z_bins, pt.z);
        return do_locate_bucket(x, y, z);
    }

    inline md::scalar squared_distance(md::point p1, md::point p2) const
    {
        return box.shortest_displacement(p1, p2).squared_norm();
    }

private:
    void init_bins(md::scalar spacing)
    {
        basic_binner<Box> binner{box, spacing};
        x_bins = binner.x_bins;
        y_bins = binner.y_bins;
        z_bins = binner.z_bins;
    }

    void init_buckets()
    {
        buckets.clear();
        buckets.resize(x_bins.count * y_bins.count * z_bins.count);

        for (std::uint32_t z = 0; z < z_bins.count; z++) {
            for (std::uint32_t y = 0; y < y_bins.count; y++) {
                for (std::uint32_t x = 0; x < x_bins.count; x++) {
                    init_bucket_adjacents(x, y, z);
                }
            }
        }
    }

    // Initializes adjacent links of a bucket that covers (x,y,z) bin.
    void init_bucket_adjacents(std::uint32_t x, std::uint32_t y, std::uint32_t z)
    {
        const auto bucket_index = do_locate_bucket(x, y, z);
        auto& bucket = buckets[bucket_index];

        const std::uint32_t dx_values[] = {0, 1, x_bins.count - 1};
        const std::uint32_t dy_values[] = {0, 1, y_bins.count - 1};
        const std::uint32_t dz_values[] = {0, 1, z_bins.count - 1};

        for (const auto dz : dz_values) {
            for (const auto dy : dy_values) {
                for (const auto dx : dx_values) {
                    const auto adj_x = detail::add_mod(x, dx, x_bins.count);
                    const auto adj_y = detail::add_mod(y, dy, y_bins.count);
                    const auto adj_z = detail::add_mod(z, dz, z_bins.count);
                    const auto adj_index = do_locate_bucket(adj_x, adj_y, adj_z);
                    if (adj_index <= bucket_index) {
                        bucket.directed_neighbors.push_back(adj_index);
                    }
                    bucket.complete_neighbors.push_back(adj_index);
                }
            }
        }

        detail::sort_unique(bucket.directed_neighbors);
        detail::sort_unique(bucket.complete_neighbors);
    }

    std::size_t do_locate_bucket(std::uint32_t x, std::uint32_t y, std::uint32_t z) const
    {
        return x + x_bins.count * (y + y_bins.count * z);
    }
};


// Periodic box is easy. Just split each axis into uniform bins.
template<>
struct basic_binner<periodic_box>
{
    detail::bin_layout x_bins;
    detail::bin_layout y_bins;
    detail::bin_layout z_bins;

    basic_binner(periodic_box box, md::scalar spacing)
    {
        x_bins = detail::define_bins(box.x_period, spacing);
        y_bins = detail::define_bins(box.y_period, spacing);
        z_bins = detail::define_bins(box.z_period, spacing);
    }
};

template<>
struct search_grid<periodic_box> : binned_search_grid<periodic_box>
{
    using binned_search_grid::binned_search_grid;
};


// xy-periodic system. We try to define z bins so that the bins do not get too
// sparse.
template<>
struct basic_binner<xy_periodic_box>
{
    detail::bin_layout x_bins;
    detail::bin_layout y_bins;
    detail::bin_layout z_bins;

    basic_binner(xy_periodic_box box, md::scalar spacing)
    {
        x_bins = detail::define_bins(box.x_period, spacing);
        y_bins = detail::define_bins(box.y_period, spacing);
        z_bins = estimate_z_bins(box, spacing);
    }

private:
    detail::bin_layout estimate_z_bins(xy_periodic_box box, md::scalar spacing) const
    {
        constexpr md::scalar min_bin_occupancy = 4.0;

        const auto volume = box.x_period * box.y_period * box.z_span;
        const auto density = md::scalar(box.particle_count) / volume;
        const auto bin_volume = x_bins.step * y_bins.step * spacing;
        const auto bin_occupancy = density * bin_volume;

        const auto target_occupancy = std::max(bin_occupancy, min_bin_occupancy);
        const auto bucket_count = md::scalar(box.particle_count) / target_occupancy;
        const auto z_mod = bucket_count / (x_bins.count * y_bins.count);

        detail::bin_layout bins;
        bins.step = spacing;
        bins.count = detail::round_uint(z_mod < 1 ? 1 : z_mod);
        return bins;
    }
};

template<>
struct search_grid<xy_periodic_box> : binned_search_grid<xy_periodic_box>
{
    using binned_search_grid::binned_search_grid;
};


// search_grid implementation for open_box. Open system tends to be sparse, so
// we use hashing instead of uniform bins to construct a grid.
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

    inline std::size_t locate_bucket(md::point pt) const
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
        buckets.resize(hash.modulus);

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
    // Creates a neighbor searcher.
    neighbor_searcher(Box box, md::scalar dcut)
        : searcher_{grid_type{box, dcut}, dcut}
    {
    }

    // Sets points to search.
    void set_points(md::array_view<md::point const> points)
    {
        searcher_.set_points(points);
    }

    // Searches neighboring points. Outputs pairs of indices of neighboring
    // points to given output iterator. Each index pair (i,j) satisfies i < j.
    // No duplicates are reported.
    template<typename OutputIterator>
    void search(OutputIterator out) const
    {
        searcher_.search(out);
    }

    // Searches neighboring points of given one.
    template<typename OutputIterator>
    void query(md::point point, OutputIterator out) const
    {
        searcher_.query(point, out);
    }

private:
    basic_neighbor_searcher<grid_type> searcher_;
};
