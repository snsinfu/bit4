#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <utility>


int main()
{
    // Broad-phase neighbor search in a 2D periodic box
    //
    // We divide the periodic box into a grid. We make the side of a grid cell
    // larger than or equal to dneigh, the neighbor distance. This way, all
    // the neighbor points of a point (center point) is contained in the cell
    // containing the center point and its surrounding cells.
    //
    //  +--+--+--+--+--+--+--+
    //  |  |  |//|//|//|  |  |
    //  +--+--+--+--+--+--+--+
    //  |  |  |//|**|//|  |  |    ** center point
    //  +--+--+--+--+--+--+--+    // surrounding cells
    //  |  |  |//|//|//|  |  |
    //  +--+--+--+--+--+--+--+

    constexpr double width = 2;
    constexpr double height = 1;
    constexpr double dneigh = 0.07;
    constexpr int npoints = 10000;

    // The cells need to be uniform for our neighbor search strategy to work.
    // However, dneigh does not always divide the box width and height. So we
    // "round up" dneigh to a valid value. Note that the cell does not need to
    // be square as long as the sides are not shorter than dneigh.

    auto const x_interval = width / std::floor(width / dneigh);
    auto const y_interval = height / std::floor(height / dneigh);

    assert(x_interval >= dneigh);
    assert(y_interval >= dneigh);

    auto const x_cells = int(std::rint(width / x_interval));
    auto const y_cells = int(std::rint(height / y_interval));

    assert(std::fabs(x_cells * x_interval - width) < 1e-6);
    assert(std::fabs(y_cells * y_interval - height) < 1e-6);

    std::clog << "Cell: " << x_interval << " x " << y_interval << '\n';
    std::clog << "Grid: " << x_cells << " x " << y_cells << '\n';

    // Generate random points in the box.

    struct point {
        double x = 0;
        double y = 0;
    };
    std::vector<point> points;

    std::mt19937 random;
    std::uniform_real_distribution<double> x_dist{0, width};
    std::uniform_real_distribution<double> y_dist{0, height};

    std::generate_n(std::back_inserter(points), npoints, [&] {
        return point{x_dist(random), y_dist(random)};
    });

    // Assign the points to the grid. Each grid cell is modeled as an array of
    // indices of the contained points.

    struct cell {
        std::vector<std::size_t> members;
    };
    std::vector<cell> cells(static_cast<std::size_t>(x_cells * y_cells));

    auto const locate = [=](int x, int y) {
        // Cell coordinate can become negative when scanning surrounding cells
        // of edge cells. We want wraparound semantics in [0,cells) but the
        // modulus operator does not work so. So let us add offsets.
        x = (x + x_cells) % x_cells;
        y = (y + y_cells) % y_cells;
        return static_cast<std::size_t>(x + y * x_cells);
    };

    auto const truncate = [=](double value) {
        return static_cast<int>(value);
    };

    for (std::size_t point_index = 0; point_index < points.size(); point_index++) {
        auto const x = truncate(points[point_index].x / x_interval);
        auto const y = truncate(points[point_index].y / y_interval);
        auto const cell_index = locate(x, y);
        cells[cell_index].members.push_back(point_index);
    }

    // Neighbor search. Scan cells and their surrounding cells, iterating over
    // the pairs of contained points.

    std::multiset<std::pair<std::size_t, std::size_t>> found_pairs;

    auto const distance = [=](point const& p1, point const& p2) {
        // Distance is computed for the nearest periodic images.
        auto const rx = std::remainder(p1.x - p2.x, width);
        auto const ry = std::remainder(p1.y - p2.y, height);
        return std::hypot(rx, ry);
    };

    for (int x = 0; x < x_cells; x++) {
    for (int y = 0; y < y_cells; y++) {
        for (int dx : {-1, 0, 1}) {
        for (int dy : {-1, 0, 1}) {

            auto const& members_i = cells[locate(x, y)].members;
            auto const& members_j = cells[locate(x + dx, y + dy)].members;

            for (auto const i : members_i) {
            for (auto const j : members_j) {

                // Prevent double counting by enforcing i < j.
                if (i >= j) {
                    continue;
                }
                assert(i < j);

                if (distance(points[i], points[j]) < dneigh) {
                    found_pairs.emplace(i, j);
                }
            }
            }
        }
        }
    }
    }

    std::clog << "Found " << found_pairs.size() << " neighbor pairs\n";

    // Brute-force search for validation.

    std::multiset<std::pair<std::size_t, std::size_t>> expected_pairs;

    for (std::size_t j = 0; j < points.size(); j++) {
    for (std::size_t i = 0; i < j; i++) {
        if (distance(points[i], points[j]) < dneigh) {
            expected_pairs.emplace(i, j);
        }
    }
    }

    std::clog << "Expecting " << expected_pairs.size() << " neighbor pairs\n";

    assert(found_pairs == expected_pairs);
}
