// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NSEARCH_HPP
#define NSEARCH_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

#include <point.hpp>


namespace detail
{
    using std::uint32_t;

    // Computes non-negative floating-point remainder of x/m. The result r
    // satisfies 0 <= r < m. Use this function to map a point to its canonical
    // image in a periodic system.
    inline double floor_mod(double x, double m)
    {
        return x - std::floor(x * (1 / m)) * m;
    }

    // Computes zero-centered floating-point remainder of x/m. The result r
    // satisfies -m/2 <= r <= m/2. Use this function to compute the distance
    // between nearest images of points in a periodic system.
    inline double round_mod(double x, double m)
    {
        return x - std::nearbyint(x * (1 / m)) * m;
    }

    // Returns the integral part of x as uint32_t.
    inline uint32_t trunc_uint(double x)
    {
        using std::int32_t;
        return static_cast<uint32_t>(static_cast<int32_t>(x));
    }

    // Rounds x to the nearest uint32_t value.
    inline uint32_t round_uint(double x)
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
        double step;
        uint32_t count;
    };

    // Creates a bin_layout that splits given span into uniform bins.
    inline bin_layout define_bins(double span, double min_step)
    {
        bin_layout bins;
        bins.count = std::max(trunc_uint(span / min_step), uint32_t(1));
        bins.step = span / double(bins.count);
        return bins;
    }

    // Computes the index of the bin in which given coordinate value falls.
    inline uint32_t locate_bin(const bin_layout& bins, double coord)
    {
        const auto span = bins.step * double(bins.count);
        const auto image = floor_mod(coord, span);
        const auto index = trunc_uint(image * (1 / bins.step));
        return index < bins.count ? index : bins.count - 1;
    }

    // Computes the squared distance between the neares images of p1 and p2 in
    // a periodic system.
    inline double nearest_squared_distance(
        cxx::point p1, cxx::point p2, cxx::vector period
    )
    {
        const auto nearest_delta = [](double delta, double period) {
            return period > 0 ? detail::round_mod(delta, period) : delta;
        };
        const auto dx = nearest_delta(p1.x - p2.x, period.x);
        const auto dy = nearest_delta(p1.y - p2.y, period.y);
        const auto dz = nearest_delta(p1.z - p2.z, period.z);
        return dx * dx + dy * dy + dz * dz;
    }

    // Computes the standard deviation of points over each axis.
    inline cxx::vector stddev_points(const std::vector<cxx::point>& points)
    {
        cxx::vector mean;
        cxx::vector mean_sq;

        for (const auto point : points) {
            const auto r = point - points[0];
            mean += r;
            mean_sq += r.hadamard(r);
        }

        if (!points.empty()) {
            mean /= double(points.size());
            mean_sq /= double(points.size());
        }

        const auto var = mean_sq - mean.hadamard(mean);
        return cxx::vector{std::sqrt(var.x), std::sqrt(var.y), std::sqrt(var.z)};
    }

    // Sort and remove duplicates in a container.
    template<typename Container>
    void sort_unique(Container& cont)
    {
        std::sort(cont.begin(), cont.end());
        cont.erase(std::unique(cont.begin(), cont.end()), cont.end());
    }
}


class xy_periodic_neighbor_searcher
{
public:
    using index_type = std::uint32_t;

    xy_periodic_neighbor_searcher(
        const std::vector<cxx::point>& points, double dcut, cxx::vector period
    )
    : dcut_{dcut}, period_{period}
    {
        init_bins(points);
        init_buckets();
        assign_points(points);
    }

    // Returns the cutoff distance.
    double cutoff_distance() const
    {
        return dcut_;
    }

    // Calls callback on each neighbor points.
    template<typename Callback>
    void foreach_neighbor(Callback callback) const
    {
        for (const auto& bucket : buckets_) {
            for (const auto adj_index : bucket.adjacents) {
                foreach_neighbor_among(bucket, buckets_[adj_index], callback);
            }
        }
    }

private:
    struct spatial_bucket
    {
        struct member
        {
            cxx::point point;
            index_type index;
        };
        std::vector<member> members;
        std::vector<index_type> adjacents;
    };

    // Initializes periodic bin layout based on dcut_ and peroid_ members and
    // point distribution.
    void init_bins(const std::vector<cxx::point>& points)
    {
        x_bins_ = detail::define_bins(period_.x, dcut_);
        y_bins_ = detail::define_bins(period_.y, dcut_);
        z_bins_ = estimate_z_bins(points);
    }

    // Estimates the optimal binning of z axis based on point distribution.
    detail::bin_layout estimate_z_bins(const std::vector<cxx::point>& points)
    {
        constexpr double min_bin_occupancy = 4.0;
        constexpr double span_per_stddev = 3.5;

        const auto stddev = detail::stddev_points(points);
        const auto z_span = span_per_stddev * stddev.z;
        const auto volume = period_.x * period_.y * z_span;
        const auto density = double(points.size()) / volume;
        const auto bin_volume = x_bins_.step * y_bins_.step * dcut_;
        const auto bin_occupancy = density * bin_volume;

        const auto target_occupancy = std::max(bin_occupancy, min_bin_occupancy);
        const auto bucket_count = double(1 + points.size()) / target_occupancy;
        const auto z_mod = bucket_count / (x_bins_.count * y_bins_.count);

        detail::bin_layout bins;
        bins.step = dcut_;
        bins.count = detail::round_uint(z_mod < 1 ? 1 : z_mod);
        return bins;
    }

    // Initializes buckets based on x/y/z_bins_ members.
    void init_buckets()
    {
        buckets_.clear();
        buckets_.resize(x_bins_.count * y_bins_.count * z_bins_.count);

        for (index_type z = 0; z < z_bins_.count; z++) {
            for (index_type y = 0; y < y_bins_.count; y++) {
                for (index_type x = 0; x < x_bins_.count; x++) {
                    init_bucket_adjacents(x, y, z);
                }
            }
        }
    }

    // Initializes adjacent links of a bucket that covers (x,y,z) bin.
    void init_bucket_adjacents(index_type x, index_type y, index_type z)
    {
        const auto bucket_index = locate_bucket(x, y, z);
        auto& bucket = buckets_[bucket_index];

        const index_type dx_values[] = {0, 1, x_bins_.count - 1};
        const index_type dy_values[] = {0, 1, y_bins_.count - 1};
        const index_type dz_values[] = {0, 1, z_bins_.count - 1};

        for (const auto dz : dz_values) {
            for (const auto dy : dy_values) {
                for (const auto dx : dx_values) {
                    const auto adj_x = detail::add_mod(x, dx, x_bins_.count);
                    const auto adj_y = detail::add_mod(y, dy, y_bins_.count);
                    const auto adj_z = detail::add_mod(z, dz, z_bins_.count);
                    const auto adj_index = locate_bucket(adj_x, adj_y, adj_z);
                    if (adj_index <= bucket_index) {
                        bucket.adjacents.push_back(adj_index);
                    }
                }
            }
        }

        detail::sort_unique(bucket.adjacents);
    }

    // Assigns points to buckets.
    void assign_points(const std::vector<cxx::point>& points)
    {
        for (index_type index = 0; index < points.size(); index++) {
            const auto point = points[index];
            const auto x = detail::locate_bin(x_bins_, point.x);
            const auto y = detail::locate_bin(y_bins_, point.y);
            const auto z = detail::locate_bin(z_bins_, point.z);
            auto& bucket = buckets_[locate_bucket(x, y, z)];
            bucket.members.push_back({point, index});
        }
    }

    // Returns the index of the bucket that the given bin (x,y,z) falls in.
    index_type locate_bucket(index_type x, index_type y, index_type z) const
    {
        return x + x_bins_.count * (y + y_bins_.count * z);
    }

    // Calls callback on each neighbor points in given bucket pair.
    template<typename Callback>
    void foreach_neighbor_among(
        const spatial_bucket& bucket1,
        const spatial_bucket& bucket2,
        Callback& callback
    ) const
    {
        const auto dcut_sq = dcut_ * dcut_;

        for (const auto member1 : bucket1.members) {
            for (const auto member2 : bucket2.members) {
                if (member1.index == member2.index) {
                    // Avoid double counting
                    break;
                }

                const auto dist_sq = detail::nearest_squared_distance(
                    member1.point, member2.point, period_
                );
                if (dist_sq < dcut_sq) {
                    callback(member1.index, member2.index);
                }
            }
        }
    }

private:
    double dcut_;
    cxx::vector period_;
    detail::bin_layout x_bins_;
    detail::bin_layout y_bins_;
    detail::bin_layout z_bins_;
    std::vector<spatial_bucket> buckets_;
};


class xy_periodic_neighbor_list
{
public:
    using index_type = std::uint32_t;
    using iterator = std::vector<std::pair<index_type, index_type>>::const_iterator;

    xy_periodic_neighbor_list(double dcut, cxx::vector period)
    : dcut_{dcut}, period_{period}
    {
    }

    iterator begin() const
    {
        return neighbors_.begin();
    }

    iterator end() const
    {
        return neighbors_.end();
    }

    void update(const std::vector<cxx::point>& points)
    {
        if (!check_consistency(points)) {
            rebuild(points);
        }
    }

private:
    bool check_consistency(const std::vector<cxx::point>& points) const
    {
        if (points.size() != prev_points_.size()) {
            return false;
        }

        // False negatives (unlisted point pairs that fall actually within dcut)
        // won't arise if the displacement from previous rebuild is less than or
        // equal to this threshold.
        const auto threshold = (verlet_radius_ - dcut_) / 2;
        const auto threshold_sq = threshold * threshold;

        if (threshold <= 0) {
            return false;
        }

        for (index_type i = 0; i < points.size(); i++) {
            if (detail::nearest_squared_distance(points[i], prev_points_[i], period_) > threshold_sq) {
                return false;
            }
        }

        return true;
    }

    void rebuild(const std::vector<cxx::point>& points)
    {
        // Heuristic: 1.2 gives fairly good performance.
        constexpr double skin_factor = 1.2;

        verlet_radius_ = dcut_ * skin_factor;
        prev_points_.assign(points.begin(), points.end());
        neighbors_.clear();

        xy_periodic_neighbor_searcher searcher{points, verlet_radius_, period_};
        searcher.foreach_neighbor([this](index_type i, index_type j) {
            neighbors_.emplace_back(i, j);
        });
    }

private:
    double dcut_;
    cxx::vector period_;
    double verlet_radius_ = 0;
    std::vector<cxx::point> prev_points_;
    std::vector<std::pair<index_type, index_type>> neighbors_;
};


#endif
