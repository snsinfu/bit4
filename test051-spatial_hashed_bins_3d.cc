#include <algorithm>
#include <chrono>
#include <ios>
#include <iostream>
#include <ostream>
#include <random>
#include <utility>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <boost/container/small_vector.hpp>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include <glm/gtx/norm.hpp>


namespace
{
    struct CollisionHash
    {
        void feed(std::size_t i, std::size_t j)
        {
            hash_ += static_cast<std::uint64_t>(i * j);
            count_ += 1;
        }

        std::uint64_t value() const
        {
            return hash_ * count_;
        }

        std::uint64_t count() const
        {
            return count_;
        }

      private:
        std::uint64_t hash_ = 0;
        std::uint64_t count_ = 0;
    };

    std::ostream& operator<<(std::ostream& os, CollisionHash const& hash)
    {
        return os << std::dec << hash.count() << ':'
                  << std::hex << hash.value()
                  << std::dec;
    }
}

namespace
{
    template<typename T>
    struct View
    {
        template<typename Container>
        inline
        View(Container&& c)
            : begin_{c.data()}, end_{c.data() + c.size()}
        {
        }

        inline
        T* begin() const noexcept
        {
            return begin_;
        }

        inline
        T* end() const noexcept
        {
            return end_;
        }

      private:
        T* begin_ = nullptr;
        T* end_   = nullptr;
    };
}

namespace
{
    using Point = glm::dvec3;
    using Vector = glm::dvec3;

    struct HashedBins
    {
        HashedBins(std::size_t nbins, double dcut)
            : bins_(nbins), dcut_{dcut}, freq_{1 / dcut}
        {
            for (std::size_t dx = nbins - 1; dx <= nbins + 1; ++dx) {
                for (std::size_t dy = nbins - 1; dy <= nbins + 1; ++dy) {
                    for (std::size_t dz = nbins - 1; dz <= nbins + 1; ++dz) {
                        deltas_.push_back(hash(dx, dy, dz));
                    }
                }
            }

            std::sort(deltas_.begin(), deltas_.end());
            deltas_.erase(std::unique(deltas_.begin(), deltas_.end()),
                          deltas_.end());
        }

        CollisionHash collision_hash(std::vector<Point> const& points)
        {
            for (Bin& bin : bins_) {
                bin.members.clear();
            }

            for (std::size_t index = 0; index < points.size(); ++index) {
                Point const point = points[index];
                Bin& bin = bins_[locate_bin(point)];
                bin.members.push_back(Bin::Member{index, point});
            }

            CollisionHash hash;

            double const dcut2 = dcut_ * dcut_;
            std::size_t const nbins = bins_.size();

            assert((nbins & (nbins - 1)) == 0); // power of two

            for (std::size_t center = 0; center < nbins; ++center) {
                View<Bin::Member const> center_members = bins_[center].members;

                for (std::size_t delta : deltas_) {
                    std::size_t const other = (center + delta) & (nbins - 1);
                    View<Bin::Member const> other_members = bins_[other].members;

                    for (Bin::Member member_i : other_members) {
                        for (Bin::Member member_j : center_members) {
                            if (member_i.index < member_j.index && glm::distance2(member_i.point, member_j.point) < dcut2) {
                                hash.feed(member_i.index, member_j.index);
                            }
                        }
                    }
                }
            }

            return hash;
        }

        void show_distribution() const
        {
            for (Bin const& bin : bins_) {
                std::cout << bin.members.size() << '\n';
            }
        }

      private:
        std::size_t locate_bin(Point point) const
        {
            double const offset = static_cast<double>(1LL << (52 / 3));

            auto const to_size_t = [](double x) {
                return static_cast<std::size_t>(static_cast<std::ptrdiff_t>(x));
            };

            std::size_t const x = to_size_t(std::nearbyint(freq_ * point.x) + offset);
            std::size_t const y = to_size_t(std::nearbyint(freq_ * point.y) + offset);
            std::size_t const z = to_size_t(std::nearbyint(freq_ * point.z) + offset);

            return hash(x, y, z);
        }

        inline
        std::size_t hash(std::size_t x, std::size_t y, std::size_t z) const
        {
            // Big prime numbers
            std::size_t const x_stride = 73856093;
            std::size_t const y_stride = 19349669;
            std::size_t const z_stride = 83492791;

            return (x * x_stride + y * y_stride + z * z_stride) % bins_.size();
        }

      private:
        struct Bin
        {
            struct Member
            {
                std::size_t index;
                Point       point;
            };
            boost::container::small_vector<Member, 8> members;
        };

        std::vector<Bin>         bins_;
        double                   dcut_;
        double                   freq_;
        std::vector<std::size_t> deltas_;
    };

    struct UniformBins
    {
        explicit
        UniformBins(double dcut)
            : dcut_{dcut}, freq_{1 / dcut}
        {
        }

        CollisionHash collision_hash(std::vector<Point> const& points)
        {
            Point min_point = points.front();
            Point max_point = points.front();

            for (Point const& point : points) {
                min_point.x = std::min(min_point.x, point.x);
                min_point.y = std::min(min_point.y, point.y);
                min_point.z = std::min(min_point.z, point.z);

                max_point.x = std::max(max_point.x, point.x);
                max_point.y = std::max(max_point.y, point.y);
                max_point.z = std::max(max_point.z, point.z);
            }

            set_domain(min_point, max_point);
            register_points(points);
            return detect_collisions(points);
        }

      private:
        void set_domain(Point min, Point max)
        {
            min_point_ = min;
            max_point_ = max;

            double const x_stride = max_point_.x - min_point_.x;
            double const y_stride = max_point_.y - min_point_.y;
            double const z_stride = max_point_.z - min_point_.z;

            nx_ = static_cast<std::size_t>(freq_ * x_stride) + 1;
            ny_ = static_cast<std::size_t>(freq_ * y_stride) + 1;
            nz_ = static_cast<std::size_t>(freq_ * z_stride) + 1;
        }

        void register_points(std::vector<Point> const& points)
        {
            bins_.resize(nx_ * ny_ * nz_);

            for (Bin& bin : bins_) {
                bin.members.clear();
            }

            for (std::size_t index = 0; index < points.size(); ++index) {
                Point const point = points[index];
                bins_[locate_bin(point)].members.push_back(Bin::Member{
                    index, point
                });
            }
        }

        CollisionHash detect_collisions(std::vector<Point> const&) const
        {
            CollisionHash hash;

            std::size_t const nbins = bins_.size();
            std::size_t const deltas[] = {
                -1 + (-1 + -1 * ny_) * nx_ + nbins,
                -1 + (-1 +  0 * ny_) * nx_ + nbins,
                -1 + (-1 + +1 * ny_) * nx_ + nbins,

                -1 + ( 0 + -1 * ny_) * nx_ + nbins,
                -1 + ( 0 +  0 * ny_) * nx_ + nbins,
                -1 + ( 0 + +1 * ny_) * nx_ + nbins,

                -1 + (+1 + -1 * ny_) * nx_ + nbins,
                -1 + (+1 +  0 * ny_) * nx_ + nbins,
                -1 + (+1 + +1 * ny_) * nx_ + nbins,

                0 + (-1 + -1 * ny_) * nx_ + nbins,
                0 + (-1 +  0 * ny_) * nx_ + nbins,
                0 + (-1 + +1 * ny_) * nx_ + nbins,

                0 + ( 0 + -1 * ny_) * nx_ + nbins,
                0 + ( 0 +  0 * ny_) * nx_ + nbins,
                0 + ( 0 + +1 * ny_) * nx_ + nbins,

                0 + (+1 + -1 * ny_) * nx_ + nbins,
                0 + (+1 +  0 * ny_) * nx_ + nbins,
                0 + (+1 + +1 * ny_) * nx_ + nbins,

                +1 + (-1 + -1 * ny_) * nx_ + nbins,
                +1 + (-1 +  0 * ny_) * nx_ + nbins,
                +1 + (-1 + +1 * ny_) * nx_ + nbins,

                +1 + ( 0 + -1 * ny_) * nx_ + nbins,
                +1 + ( 0 +  0 * ny_) * nx_ + nbins,
                +1 + ( 0 + +1 * ny_) * nx_ + nbins,

                +1 + (+1 + -1 * ny_) * nx_ + nbins,
                +1 + (+1 +  0 * ny_) * nx_ + nbins,
                +1 + (+1 + +1 * ny_) * nx_ + nbins,
            };

            double const dcut2 = dcut_ * dcut_;

            for (std::size_t center = 0; center < nbins; ++center) {
                View<Bin::Member const> center_members = bins_[center].members;

                for (std::size_t delta : deltas) {
                    std::size_t const other = (center + delta) % nbins;
                    View<Bin::Member const> other_members = bins_[other].members;

                    for (Bin::Member member_i : center_members) {
                        for (Bin::Member member_j : other_members) {
                            if (member_i.index < member_j.index && glm::distance2(member_i.point, member_j.point) < dcut2) {
                                hash.feed(member_i.index, member_j.index);
                            }
                        }
                    }
                }
            }

            return hash;
        }

        inline
        std::size_t locate_bin(Point point) const
        {
            auto const to_size_t = [](double x) {
                return static_cast<std::size_t>(static_cast<std::ptrdiff_t>(x));
            };

            std::size_t const x = to_size_t(freq_ * (point.x - min_point_.x));
            std::size_t const y = to_size_t(freq_ * (point.y - min_point_.y));
            std::size_t const z = to_size_t(freq_ * (point.z - min_point_.z));

            return x + (y + z * ny_) * nx_;
        }

      private:
        struct Bin
        {
            struct Member
            {
                std::size_t index;
                Point       point;
            };
            boost::container::small_vector<Member, 4> members;
        };

        std::vector<Bin> bins_;
        double           dcut_;
        double           freq_;
        Point            min_point_;
        Point            max_point_;
        std::size_t      nx_ = 0;
        std::size_t      ny_ = 0;
        std::size_t      nz_ = 0;
    };
}

namespace
{
    CollisionHash brute_force(std::vector<Point> const& points, double radius)
    {
        double const dcut = 2 * radius;
        double const dcut2 = dcut * dcut;

        CollisionHash hash;

        for (std::size_t j = 0; j < points.size(); ++j) {
            for (std::size_t i = 0; i < j; ++i) {
                if (glm::distance2(points[i], points[j]) < dcut2) {
                    hash.feed(i, j);
                }
            }
        }

        return hash;
    }

    CollisionHash uniform_bins(std::vector<Point> const& points, double radius)
    {
        return UniformBins{2 * radius}.collision_hash(points);
    }

    CollisionHash spatial_hashing(std::vector<Point> const& points, double radius)
    {
        std::size_t const nbins_heuristic = 16384;
        return HashedBins{nbins_heuristic, 2 * radius}.collision_hash(points);
    }
}

namespace
{
    double halton(unsigned long i, unsigned long radix)
    {
        double result = 0;
        double fraction = 1;

        for (; i != 0; i /= radix)
        {
            fraction /= radix;
            result += fraction * static_cast<double>(i % radix);
        }

        return result;
    }

    template<typename RNG>
    std::vector<Point> generate_uniform_points(std::size_t n_points, RNG& engine)
    {
        std::vector<Point> points;
        std::uniform_real_distribution<double> coord{-1, 1};

        for (std::size_t i = 0; i < n_points; ++i) {
            double const x = coord(engine);
            double const y = coord(engine);
            double const z = coord(engine);
            points.emplace_back(x, y, z);
        }

        return points;
    }

    std::vector<Point> generate_quasirandom_points(std::size_t n_points)
    {
        std::vector<Point> points;

        for (std::size_t i = 0; i < n_points; ++i) {
            double const x = 2 * halton(i, 2) - 1;
            double const y = 2 * halton(i, 3) - 1;
            double const z = 2 * halton(i, 5) - 1;
            points.emplace_back(x, y, z);
        }

        return points;
    }

    template<typename RNG>
    std::vector<Point> generate_bounded_uniform_points(std::size_t n_points, RNG& engine)
    {
        std::vector<Point> points;
        std::uniform_real_distribution<double> coord{-1, 1};

        while (points.size() < n_points) {
            double const x = coord(engine);
            double const y = coord(engine);
            double const z = coord(engine);
            if (x * x + y * y + z * z > 1) {
                continue;
            }
            points.emplace_back(x, y, z);
        }

        return points;
    }

    template<typename RNG>
    std::vector<Point> generate_normal_points(std::size_t n_points, RNG& engine)
    {
        std::vector<Point> points;
        std::normal_distribution<double> coord{0, 0.5};

        for (std::size_t i = 0; i < n_points; ++i) {
            double const x = coord(engine);
            double const y = coord(engine);
            double const z = coord(engine);
            points.emplace_back(x, y, z);
        }

        return points;
    }

    template<typename RNG>
    std::vector<Point> generate_random_walk(std::size_t n_points, double stride, RNG& engine)
    {
        std::vector<Point> points;

        points.emplace_back(); // initial point

        std::normal_distribution<double> normal;
        for (std::size_t i = 0; i < n_points; ++i) {
            Vector direction = {normal(engine), normal(engine), normal(engine)};
            direction /= direction.length();
            points.push_back(points.back() + stride * direction);
        }

        return points;
    }
}

namespace
{
    template<typename Rep, typename Period>
    double to_seconds(std::chrono::duration<Rep, Period> dur)
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(dur).count();
    }
}

int main()
{
    std::size_t const n_points = 60000;
    double const radius = 0.03;

    std::mt19937_64 engine;
    std::vector<Point> points;

    auto const bench = [&](char const* name, auto func) {
        using Clock = std::chrono::steady_clock;

        Clock::time_point const start = Clock::now();
        CollisionHash const hash = func(points, radius);
        Clock::duration const elapsed = Clock::now() - start;
        double const elapsed_ms = 1000 * to_seconds(elapsed);

        std::cout << name
                  << ":\t"
                  << hash
                  << '\t'
                  << elapsed_ms << " ms"
                  << '\n';
    };

    std::clog << "Warming up.";
    for (int i = 0; i < 3; ++i) {
        points = generate_uniform_points(n_points, engine);
        if (brute_force(points, radius).value() % 2 == 0) {
            std::clog << ',';
        } else {
            std::clog << '.';
        }
    }
    std::clog << '\n';

    std::cout << "\nUniform\n";
    points = generate_uniform_points(n_points, engine);
    bench("Brute", brute_force);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);

    std::cout << "\nQuasirandom\n";
    points = generate_quasirandom_points(n_points);
    bench("Brute", brute_force);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);

    std::cout << "\nBounded\n";
    points = generate_bounded_uniform_points(n_points, engine);
    bench("Brute", brute_force);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);

    std::cout << "\nNormal\n";
    points = generate_normal_points(n_points, engine);
    bench("Brute", brute_force);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);

    std::cout << "\nRandom walk\n";
    points = generate_random_walk(n_points, 2 * radius, engine);
    bench("Brute", brute_force);
    bench("Bin", uniform_bins);
    bench("Hash", spatial_hashing);
}
