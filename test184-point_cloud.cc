// Application of the Loan Pattern: Updating collision information on modifying
// particle positions in point cloud.

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
            : ptr_{ptr}, size_{size}
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
}

namespace geo
{
    struct point
    {
        double x = 0;
        double y = 0;
        double z = 0;
    };

    double squared_distance(point const& p, point const& q)
    {
        double const dx = p.x - q.x;
        double const dy = p.y - q.y;
        double const dz = p.z - q.z;
        return dx*dx + dy*dy + dz*dz;
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
            points.push_back(point{
                x * spacing, y * spacing, z * spacing
            });
        }
        assert(points.size() == count);

        return points;
    }

    struct point_cloud
    {
        explicit point_cloud(std::size_t count, double diameter)
            : points_{generate_default_cloud(count, diameter)}
            , diameter_{diameter}
        {
            update();
        }

        ext::array_view<point const> view() const
        {
            return {points_.data(), points_.size()};
        }

        ext::array_view<std::pair<std::size_t, std::size_t> const> colliding_pairs() const
        {
            return {collisions_.data(), collisions_.size()};
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

            for (std::size_t i = 0; i < points_.size(); ++i) {
                for (std::size_t j = 0; j < i; ++j) {
                    if (squared_distance(points_[i], points_[j]) < diameter_sq) {
                        collisions_.emplace_back(i, j);
                    }
                }
            }
        }

      private:
        std::vector<point> points_;
        double diameter_;
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
    geo::point_cloud cloud{10000, 1.0};
    std::mt19937_64 engine;

    std::cout << cloud.colliding_pairs().size() << '\n';
    for (int step = 0; step < 10; ++step) {
        displace_points(cloud, engine);
        std::cout << cloud.colliding_pairs().size() << '\n';
    }
}
