#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <cmath>
#include <cstddef>
#include <cstdint>

#include <gsl/span>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

using Index = std::uint32_t;
using Scalar = double;
using Point = glm::dvec3;
using Vector = glm::dvec3;

struct IndexPair
{
    Index i = 0;
    Index j = 0;

    IndexPair() = default;

    IndexPair(Index i, Index j)
        : i{i}, j{j}
    {
    }
};

struct Interval
{
    Scalar begin = 0;
    Scalar end   = 0;

    Interval() = default;

    Interval(Scalar begin, Scalar end)
        : begin{begin}, end{end}
    {
    }

    bool contains(Scalar x) const
    {
        return begin <= x && x < end;
    }

    bool covers(Interval const& other) const
    {
        return begin <= other.begin && other.end <= end;
    }
};

bool overlaps(Interval const& a, Interval const& b)
{
    return a.contains(b.begin) || b.contains(a.begin);
}

std::mt19937_64 make_engine()
{
    std::random_device src;
    std::seed_seq seed{src(), src(), src(), src()};
    return std::mt19937_64{seed};
}

std::vector<Point> generate_points(Index num, std::mt19937_64& engine)
{
    std::vector<Point> points;

    std::uniform_real_distribution<Scalar> step{-1, 1};
    Point point;
    for (Index i = 0; i < num; ++i) {
        point += Vector{step(engine), step(engine), step(engine)};
        points.push_back(point);
    }
    return points;
}

template<typename T>
void sort2(T& x, T& y) {
    bool const is_sorted = (x < y);
    T min = std::move(is_sorted ? x : y);
    T max = std::move(is_sorted ? y : x);
    x = std::move(min);
    y = std::move(max);
}

template<typename T>
T median(T x, T y, T z) {
    sort2(x, y);
    sort2(y, z);
    sort2(x, y);
    return y;
}

struct StreamingBoxIntersect
{
    static constexpr Scalar min = std::numeric_limits<Scalar>::min();
    static constexpr Scalar max = std::numeric_limits<Scalar>::max();

    void compute(gsl::span<Interval> A, gsl::span<Interval> B)
    {
        scan(A, B, min, max);
    }

    void scan(
        gsl::span<Interval> I,
        gsl::span<Interval> P,
        Scalar lo,
        Scalar hi
    )
    {
        if (I.empty() || P.empty() || hi <= lo) {
            return;
        }

        if (I.size() < 32 || P.size() < 32) {
            return;
        }

        std::ptrdiff_t const Im_idx = std::distance(
            I.begin(),
            std::partition(I.begin(), I.end(), [=](Interval const& x) {
                return x.covers(Interval{lo, hi});
            })
        );
        gsl::span<Interval> const I_Im = I.subspan(Im_idx);

        Scalar const mi = median(
            P[0].begin,
            P[P.size() / 2].begin,
            P[P.size() - 1].begin
        );

        std::ptrdiff_t const P_idx = std::distance(
            P.begin(),
            std::partition(P.begin(), P.end(), [=](Interval const& x) {
                return x.begin < mi;
            })
        );
        gsl::span<Interval> const Pl = P.subspan(0, P_idx);
        gsl::span<Interval> const Pr = P.subspan(P_idx);

        std::ptrdiff_t const Il_idx = std::distance(
            I_Im.begin(),
            std::partition(I_Im.begin(), I_Im.end(), [=](Interval const& x) {
                return overlaps(x, Interval{lo, mi});
            })
        );
        gsl::span<Interval> const Il = I_Im.subspan(0, Il_idx);

        std::ptrdiff_t const Ir_idx = std::distance(
            I_Im.begin(),
            std::partition(I_Im.begin(), I_Im.end(), [=](Interval const& x) {
                return overlaps(x, Interval{mi, hi});
            })
        );
        gsl::span<Interval> const Ir = I_Im.subspan(0, Ir_idx);

        scan(Il, Pl, lo, mi);
        scan(Ir, Pr, mi, hi);
    }
};

int main()
{
    std::mt19937_64 engine = make_engine();
    std::vector<Point> points = generate_points(10000, engine);
    std::vector<Interval> intervals;
    for (Point point : points) {
        intervals.push_back(Interval{point.x - 0.1, point.x + 0.1});
    }

    StreamingBoxIntersect intersect;
    intersect.compute(intervals, intervals);
}
