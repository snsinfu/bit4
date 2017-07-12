#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <unordered_set>
#include <vector>

#include <cmath>
#include <cstddef>

#include <gsl/span>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

void sweep(gsl::span<glm::dvec2 const> points, double radius)
{
    struct SweepEdge
    {
        enum class Type
        {
            enter, exit
        };

        glm::ivec2  coord;
        std::size_t index;
        Type        type;
    };

    double const binfreq = 0.5 / radius;

    std::vector<SweepEdge> edges;
    edges.reserve(points.size() * 2);

    for (std::size_t i = 0; i < static_cast<std::size_t>(points.size()); ++i) {
        edges.push_back(SweepEdge{
            {static_cast<int>(binfreq * (points[i].x - radius)),
             static_cast<int>(binfreq * (points[i].y - radius))},
            i,
            SweepEdge::Type::enter
        });
        edges.push_back(SweepEdge{
            {static_cast<int>(binfreq * (points[i].x + radius)),
             static_cast<int>(binfreq * (points[i].y + radius))},
            i,
            SweepEdge::Type::enter
        });
    }

    std::sort(edges.begin(), edges.end(), [](SweepEdge const& e1, SweepEdge const& e2) {
        // Lexicographic ordering
        return std::tie(e1.coord.x, e1.coord.y) < std::tie(e2.coord.x, e2.coord.y);
    });

    std::unordered_set<std::size_t> active_boxes;
    std::size_t count = 0;

    for (SweepEdge const& edge : edges) {
        if (edge.type == SweepEdge::Type::enter) {
            for (std::size_t other : active_boxes) {
                count += 1;
                (void) other;
            }
        }

        switch (edge.type) {
          case SweepEdge::Type::enter:
            active_boxes.insert(edge.index);
            break;

          case SweepEdge::Type::exit:
            active_boxes.erase(active_boxes.find(edge.index));
            break;
        }
    }

    std::cout << count << '\n';
}

int main()
{
    std::size_t const n_points = 100000;
    double const radius = 0.005;

    std::vector<glm::dvec2> points;

    std::mt19937_64 engine;
    std::uniform_real_distribution<double> coord{-1, 1};
    for (std::size_t i = 0; i < n_points; ++i) {
        double const x = coord(engine);
        double const y = coord(engine);
        points.emplace_back(x, y);
    }

    sweep(points, radius);
}
