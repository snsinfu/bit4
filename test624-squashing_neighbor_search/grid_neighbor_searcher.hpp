#ifndef INCLUDED_GRID_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_GRID_NEIGHBOR_SEARCHER_HPP

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"


namespace md
{
    class grid_neighbor_searcher
    {
    public:
        explicit grid_neighbor_searcher(md::scalar dcut)
            : dcut_(dcut)
        {
        }

        void set_points(md::array_view<md::point const> points)
        {
            md::point min;
            md::point max;

            for (auto pt : points) {
                min.x = std::min(min.x, pt.x);
                min.y = std::min(min.y, pt.y);
                min.z = std::min(min.z, pt.z);

                max.x = std::max(max.x, pt.x);
                max.y = std::max(max.y, pt.y);
                max.z = std::max(max.z, pt.z);
            }

            auto const dcut_inv = 1 / dcut_;

            auto const nx = int(std::ceil((max.x - min.x) * dcut_inv));
            auto const ny = int(std::ceil((max.y - min.y) * dcut_inv));
            auto const nz = int(std::ceil((max.z - min.z) * dcut_inv));

            grid_.resize(md::index(nx * ny * nz));

            for (int x = 0; x < nx; x++) {
                for (int y = 0; y < ny; y++) {
                    for (int z = 0; z < nz; z++) {

                        auto const loc = x + nx * (y + ny * z);
                        auto& cell = grid_[md::index(loc)];

                        cell.members.clear();
                        cell.adjacent_count = 0;

                        for (int dx : {-1, 0, 1}) {
                            auto const ax = x + dx;
                            if (ax < 0 || ax >= nx) {
                                continue;
                            }

                            for (int dy : {-1, 0, 1}) {
                                auto const ay = y + dy;
                                if (ay < 0 || ay >= ny) {
                                    continue;
                                }

                                for (int dz : {-1, 0, 1}) {
                                    auto const az = z + dz;
                                    if (az < 0 || az >= nz) {
                                        continue;
                                    }

                                    auto const adj = ax + nx * (ay + ny * az);

                                    cell.adjacent_cells[cell.adjacent_count++] = md::index(adj);
                                }
                            }
                        }
                    }
                }
            }

            for (md::index idx = 0; idx < points.size(); idx++) {
                auto const pt = points[idx];

                auto const x = int((pt.x - min.x) * dcut_inv);
                auto const y = int((pt.y - min.y) * dcut_inv);
                auto const z = int((pt.z - min.z) * dcut_inv);
                auto const loc = x + nx * (y + ny * z);
                auto& cell = grid_[md::index(loc)];

                cell.members.push_back({idx, pt});
            }
        }

        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            auto const dcut2 = dcut_ * dcut_;

            for (auto const& cell : grid_) {
                for (auto const mi : cell.members) {
                    for (md::index a = 0; a < cell.adjacent_count; a++) {
                        auto const& adj = grid_[cell.adjacent_cells[a]];

                        for (auto const mj : adj.members) {
                            if (mi.index >= mj.index) {
                                continue;
                            }

                            if (md::squared_distance(mi.point, mj.point) > dcut2) {
                                continue;
                            }

                            *out++ = std::make_pair(mi.index, mj.index);
                        }
                    }
                }
            }
        }

    private:
        struct cell_t
        {
            struct member
            {
                md::index index;
                md::point point;
            };
            std::vector<member> members;

            md::index adjacent_count;
            md::index adjacent_cells[27];
        };

    private:
        md::scalar dcut_;
        std::vector<cell_t> grid_;
    };
}

#endif
