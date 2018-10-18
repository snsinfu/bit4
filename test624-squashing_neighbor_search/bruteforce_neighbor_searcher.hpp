#ifndef INCLUDED_BRUTEFORCE_NEIGHBOR_SEARCHER_HPP
#define INCLUDED_BRUTEFORCE_NEIGHBOR_SEARCHER_HPP

#include <utility>

#include "array_view.hpp"
#include "point.hpp"
#include "types.hpp"


namespace md
{
    class bruteforce_neighbor_searcher
    {
    public:
        explicit bruteforce_neighbor_searcher(md::scalar dcut)
            : dcut_{dcut}
        {
        }

        void set_points(md::array_view<md::point const> points)
        {
            points_ = points;
        }

        template<typename OutputIterator>
        void search(OutputIterator out) const
        {
            md::scalar const dcut2 = dcut_ * dcut_;

            for (md::index j = 0; j < points_.size(); j++) {
                for (md::index i = 0; i < j; i++) {
                    if (md::squared_distance(points_[i], points_[j]) < dcut2) {
                        *out++ = std::make_pair(i, j);
                    }
                }
            }
        }

    private:
        md::scalar dcut_;
        md::array_view<md::point const> points_;
    };
}

#endif
