#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
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

template<typename T>
gsl::span<T> subspan(
    gsl::span<T> const& s,
    typename gsl::span<T>::const_iterator beg,
    typename gsl::span<T>::const_iterator end
)
{
    return gsl::span<T>{s.data() + std::distance(s.cbegin(), beg), end - beg};
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
        gsl::span<Interval> const& I,
        gsl::span<Interval> const& P,
        Scalar lo,
        Scalar hi
    )
    {
        if (I.empty() || P.empty() || hi <= lo) {
            return;
        }

        if (I.size() < 8 || P.size() < 8) {
            brute_force_scan(I, P);
            return;
        }

        // I = Im | Im_complement
        auto const Im_end = std::partition(I.begin(), I.end(), [=](Interval const& iv) {
            return iv.covers(Interval{lo, hi});
        });

        // P = P_L | P_R
        auto const mi = median(
            P[0].begin,
            P[P.size() / 2].begin,
            P[P.size() - 1].begin
        );
        auto const P_mid = std::partition(P.begin(), P.end(), [=](Interval const& iv) {
            return iv.begin < mi;
        });
        auto const P_L = subspan(P, P.begin(), P_mid);
        auto const P_R = subspan(P, P_mid, P.end());

        // Im_complement = LR_complement | LR
        auto const LR_beg = std::partition(Im_end, I.end(), [=](Interval const& iv) {
            return iv.end < lo || hi <= iv.begin;
        });

        // LR = L | L_complement
        auto const L_end = std::partition(LR_beg, I.end(), [=](Interval const& iv) {
            return overlaps(iv, Interval{lo, mi});
        });
        auto const I_L = subspan(I, LR_beg, L_end);
        scan(I_L, P_L, lo, hi);

        // LR = R_complement | R  (L may overlap R)
        auto const R_beg = std::partition(LR_beg, L_end, [=](Interval const& iv) {
            return iv.end < mi;
        });
        auto const I_R = subspan(I, R_beg, I.end());
        scan(I_R, P_R, lo, hi);
    }

    void brute_force_scan(
        gsl::span<Interval> const& I,
        gsl::span<Interval> const& P
    )
    {
        for (Interval interval : I) {
            for (Interval target : P) {
                if (interval.contains(target.begin)) {
                    std::cout << interval.begin
                              << '\t'
                              << target.begin
                              << '\n';
                }
            }
        }
    }
};

int main()
{
    std::mt19937_64 engine;
    std::vector<Point> points = generate_points(1000, engine);

    std::vector<Interval> intervals;
    for (Point point : points) {
        intervals.push_back(Interval{point.x - 0.1, point.x + 0.1});
    }
    std::vector<Interval> targets = intervals;

    StreamingBoxIntersect intersect;
    intersect.compute(intervals, targets);
}
