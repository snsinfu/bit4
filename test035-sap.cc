#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <gsl/span>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

#include <pdqsort.h>


using Index = std::size_t;
using Scalar = double;
using Point = glm::dvec3;
using Vector = glm::dvec3;

struct IndexPair {
    Index i, j;

    IndexPair(Index i, Index j) : i{i}, j{j} {
    }
};

std::vector<IndexPair> sweep_and_prune(
    gsl::span<Point const> points,
    Scalar radius
) {
    struct SweepPoint {
        enum class Type {
            begin, end
        };

        Scalar coord;
        Index  index;
        Type   type;
    };

    Index const num_points = static_cast<Index>(points.size());

    std::vector<SweepPoint> x_sweep;
    x_sweep.reserve(num_points * 2);
    for (Index i = 0; i < num_points; ++i) {
        x_sweep.push_back({points[i].x - radius, i, SweepPoint::Type::begin});
        x_sweep.push_back({points[i].x + radius, i, SweepPoint::Type::end  });
    }

    pdqsort_branchless(
        x_sweep.begin(), x_sweep.end(),
        [&](SweepPoint const& a, SweepPoint const& b) {
            return a.coord < b.coord;
        }
    );

    std::vector<Index> slot_map(points.size());
    std::vector<Index> active;
    std::vector<IndexPair> collisions;

    for (SweepPoint s : x_sweep) {
        if (s.type == SweepPoint::Type::begin) {
            Scalar const dcut2 = (radius * 2) * (radius * 2);
            for (Index i : active) {
                if (glm::distance2(points[s.index], points[i]) < dcut2) {
                    collisions.push_back(IndexPair{s.index, i});
                }
            }
        }

        switch (s.type) {
          case SweepPoint::Type::begin:
            slot_map[s.index] = active.size();
            active.push_back(s.index);
            break;

          case SweepPoint::Type::end:
            active[slot_map[s.index]] = active.back();
            slot_map[active.back()] = slot_map[s.index];
            active.pop_back();
            break;
        }
    }

    return collisions;
}

std::vector<Point> generate_points(Index num, std::uint64_t seed) {
    std::vector<Point> points;

    std::mt19937_64 engine{seed};
    std::uniform_real_distribution<double> step{-0.1, 0.1};
    Point pos = {0, 0, 0};

    for (Index i = 0; i < num; ++i) {
        pos += Vector{step(engine), step(engine), step(engine)};
        points.push_back(pos);
    }
    return points;
}

int main() {
    std::vector<Point> const points = generate_points(100000, 1234);
    std::vector<IndexPair> const collisions = sweep_and_prune(points, 0.1);
    std::cout << collisions.size() << '\n';
}
