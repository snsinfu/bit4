// Optimized version of test187. ~20% speed up.

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

namespace ext
{
    template<typename T>
    struct array_view
    {
        array_view(T* ptr, std::size_t size)
            : ptr_{ptr}
            , size_{size}
        {
        }

        T* data() const noexcept
        {
            return ptr_;
        }

        std::size_t size() const noexcept
        {
            return size_;
        }

        T& operator[](std::size_t index)
        {
            return ptr_[index];
        }

        T const& operator[](std::size_t index) const
        {
            return ptr_[index];
        }

        T* begin() const noexcept
        {
            return ptr_;
        }

        T* end() const noexcept
        {
            return ptr_ + size_;
        }

      private:
        T* ptr_;
        std::size_t size_;
    };

    template<typename T, typename Allocator>
    array_view<T> view(std::vector<T, Allocator>& vec)
    {
        return array_view<T>{vec.data(), vec.size()};
    }

    template<typename T, typename Allocator>
    array_view<T const> view(std::vector<T, Allocator> const& vec)
    {
        return array_view<T const>{vec.data(), vec.size()};
    }

    template<typename T, typename Allocator>
    array_view<T const> const_view(std::vector<T, Allocator> const& vec)
    {
        return array_view<T const>{vec.data(), vec.size()};
    }
}

namespace geo
{
    struct point
    {
        double x = 0;
        double y = 0;
        double z = 0;
    };

    double squared_distance(point p, point q)
    {
        double const dx = p.x - q.x;
        double const dy = p.y - q.y;
        double const dz = p.z - q.z;
        return dx * dx + dy * dy + dz * dz;
    }

    std::vector<point> generate_default_cloud(std::size_t count, double diameter)
    {
        std::vector<point> points;
        points.reserve(count);

        double const spacing = diameter * 2;
        double const cbrt_count = std::cbrt(static_cast<double>(count));
        auto const side_count = static_cast<std::size_t>(std::ceil(cbrt_count));
        assert(side_count * side_count * side_count >= count);

        for (std::size_t index = 0; index < count; ++index) {
            std::size_t const x = index % side_count;
            std::size_t const y = index / side_count % side_count;
            std::size_t const z = index / side_count / side_count;
            points.push_back(point{x * spacing, y * spacing, z * spacing});
        }
        assert(points.size() == count);

        return points;
    }

    struct spatial_hash
    {
        spatial_hash(std::size_t buckets, double interval)
            : buckets_(buckets)
            , reciprocal_interval_{1 / interval}
        {
            init_hash_deltas();
        }

        void assign(ext::array_view<point const> points)
        {
            for (auto& bucket : buckets_) {
                bucket.members.clear();
            }

            for (std::size_t index = 0; index < points.size(); ++index) {
                auto const point = points[index];
                auto& bucket = buckets_[locate_bucket(point)];
                bucket.members.push_back(bucket::member{index, point});
            }
        }

        template<typename F>
        void for_each_neighbor(F func)
        {
            std::size_t const n_buckets = buckets_.size();

            for (std::size_t center = 0; center < n_buckets; ++center) {
                for (std::size_t delta : hash_deltas_) {
                    for_each_pair(center, (center + delta) % n_buckets, func);
                }
            }
        }

      private:
        template<typename F>
        void for_each_pair(std::size_t center, std::size_t other, F& func)
        {
            for (auto const member_i : buckets_[other].members) {
                for (auto const member_j : buckets_[center].members) {
                    if (member_i.index >= member_j.index) {
                        continue;
                    }
                    func(member_i.index, member_i.position,
                         member_j.index, member_j.position);
                }
            }
        }

        std::size_t locate_bucket(point const& pt) const
        {
            auto const to_size_t = [](double x) {
                // double -> signed integer conversion is fast
                return static_cast<std::size_t>(static_cast<std::ptrdiff_t>(x));
            };
            auto const x = to_size_t(std::nearbyint(reciprocal_interval_ * pt.x));
            auto const y = to_size_t(std::nearbyint(reciprocal_interval_ * pt.y));
            auto const z = to_size_t(std::nearbyint(reciprocal_interval_ * pt.z));

            return linear_hash(x, y, z) % buckets_.size();
        }

        std::size_t linear_hash(std::size_t x, std::size_t y, std::size_t z) const
        {
            // Big prime numbers (openssl prime -generate -bits 31)
            std::size_t const x_stride = 2027826869;
            std::size_t const y_stride = 1872466073;
            std::size_t const z_stride = 1694793383;

            return x * x_stride + y * y_stride + z * z_stride;
        }

        void init_hash_deltas()
        {
            hash_deltas_.clear();

            std::size_t const n_buckets = buckets_.size();
            std::size_t const coord_deltas[] = {n_buckets - 1, n_buckets, n_buckets + 1};

            for (std::size_t dx : coord_deltas) {
                for (std::size_t dy : coord_deltas) {
                    for (std::size_t dz : coord_deltas) {
                        hash_deltas_.push_back(linear_hash(dx, dy, dz) % n_buckets);
                    }
                }
            }

            std::sort(std::begin(hash_deltas_), std::end(hash_deltas_));
            hash_deltas_.erase(std::unique(std::begin(hash_deltas_), std::end(hash_deltas_)),
                std::end(hash_deltas_));
        }

      private:
        struct bucket
        {
            struct member
            {
                std::size_t index;
                point position;
            };
            std::vector<member> members;
        };
        std::vector<bucket> buckets_;
        std::vector<std::size_t> hash_deltas_;
        double reciprocal_interval_;
    };

    struct point_cloud
    {
        explicit point_cloud(std::size_t count, double diameter)
            : points_{generate_default_cloud(count, diameter)}
            , diameter_{diameter}
            , hash_{count / 12 + 1, diameter}
        {
            update();
        }

        ext::array_view<point const> view() const
        {
            return ext::view(points_);
        }

        ext::array_view<std::pair<std::size_t, std::size_t> const> colliding_pairs() const
        {
            return ext::view(collisions_);
        }

        template<typename F>
        void mutate(F func)
        {
            func(ext::array_view<point>{points_.data(), points_.size()});
            update();
        }

      private:
        void update()
        {
            double const diameter_sq = diameter_ * diameter_;
            collisions_.clear();

#if WITH_HASH
            hash_.assign(ext::const_view(points_));
            hash_.for_each_neighbor([&](auto i, auto pi, auto j, auto pj) {
                if (squared_distance(pi, pj) < diameter_sq) {
                    collisions_.emplace_back(i, j);
                }
            });
#else
            for (std::size_t i = 0; i < points_.size(); ++i) {
                for (std::size_t j = 0; j < i; ++j) {
                    if (squared_distance(points_[i], points_[j]) < diameter_sq) {
                        collisions_.emplace_back(i, j);
                    }
                }
            }
#endif
        }

      private:
        std::vector<point> points_;
        double diameter_;
        spatial_hash hash_;
        std::vector<std::pair<std::size_t, std::size_t>> collisions_;
    };
}

namespace
{
    void displace_points(geo::point_cloud& cloud, std::mt19937_64& engine)
    {
        std::normal_distribution<double> displace{0.0, 1.0};

        cloud.mutate([&](auto points) {
            for (auto& point : points) {
                point.x += displace(engine);
                point.y += displace(engine);
                point.z += displace(engine);
            }
        });
    }
}

int main()
{
    geo::point_cloud cloud{100000, 1.0};
    std::mt19937_64 engine;

    std::cout << cloud.colliding_pairs().size() << '\n';
    for (int step = 0; step < 50; ++step) {
        displace_points(cloud, engine);
        std::cout << cloud.colliding_pairs().size() << '\n';
    }
}
