#pragma once

#include <utility>
#include <vector>

#include <md.hpp>


namespace md
{
    template<typename Box>
    class neighbor_pairs
    {
        using iterator = std::vector<std::pair<md::index, md::index>>::const_iterator;

    public:
        neighbor_pairs()
            : searcher_{box_, prev_verlet_radius_} // FIXME: Poor default
        {
        }

        void set_box(Box box)
        {
            box_ = box;
            tainted_ = true;
        }

        void set_cutoff_distance(md::scalar dcut)
        {
            dcut_ = dcut;
            tainted_ = true;
        }

        template<typename R>
        void set_targets(R const& range)
        {
        }

        void update(md::array_view<md::point const> points)
        {
            if (!check_consistency(points)) {
                rebuild(points);
            }
        }

        // Range interface.
        iterator begin() const
        {
            return pairs_.begin();
        }

        iterator end() const
        {
            return pairs_.end();
        }

    private:
        // Checks if the previously created neighbor list is still usable with
        // the given configuration.
        bool check_consistency(md::array_view<md::point const> points) const
        {
            if (points.size() != prev_points_.size()) {
                return false;
            }

            // False negatives (unlisted point pairs that fall actually within
            // dcut) won't arise if the displacement from previous rebuild is
            // less than or equal to this threshold.
            md::scalar const threshold = (prev_verlet_radius_ - dcut_) / 2;

            if (threshold <= 0) {
                return false;
            }

            for (md::index i = 0; i < points.size(); i++) {
                md::vector const disp = box_.shortest_displacement(points[i], prev_points_[i]);
                if (disp.squared_norm() > threshold * threshold) {
                    return false;
                }
            }
            return true;
        }

        // Rebuilds the neighbor list.
        void rebuild(md::array_view<md::point const> points)
        {
            detail::set_box_hints(box_, points);

            // Let v be the verlet factor. The cost of list construction scales
            // with v^3, while the benefit of list reuse scales with (v-1). So
            // the overall cost scales with v^3/(v-1). The minimum cost is then
            // achieved when v = 1.5.
            static constexpr md::scalar verlet_factor = 1.5;
            md::scalar const verlet_radius = verlet_factor * dcut_;

            // Neighbor searcher is expensive to construct. Reuse previous one
            // if possible.
            if (tainted_) {
                searcher_ = md::neighbor_searcher_v2<Box>{box_, verlet_radius};
                tainted_ = false;
            }

            prev_points_.assign(points.begin(), points.end());
            pairs_.clear();
            searcher_.set_points(prev_points_);
            searcher_.search(std::back_inserter(pairs_));

            prev_verlet_radius_ = verlet_radius;
        }

    private:
        Box box_;
        bool tainted_ = false;
        md::scalar dcut_ = 0.1;
        md::scalar prev_verlet_radius_ = 0.1;
        md::neighbor_searcher_v2<Box> searcher_;
        std::vector<md::point> prev_points_;
        std::vector<std::pair<md::index, md::index>> pairs_;
    };
}
