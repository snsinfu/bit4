#pragma once

#include <md.hpp>


namespace md
{
    template<typename Derived>
    class pairwise_forcefield : public virtual md::forcefield
    {
    private:
        md::scalar compute_energy(md::system const& system) override
        {
            auto const& pair_list = derived().pair_list(system);
            auto const box = derived().box(system);
            auto const positions = system.view_positions();

            md::scalar sum = 0;

            for (auto const pair : pair_list) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const pot = derived().pairwise_potential(system, i, j);
                auto const r = box.shortest_displacement(positions[i], positions[j]);

                sum += pot.evaluate_energy(r);
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const& pair_list = derived().pair_list(system);
            auto const box = derived().box(system);
            auto const positions = system.view_positions();

            for (auto const pair : pair_list) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const pot = derived().pairwise_potential(system, i, j);
                auto const r = box.shortest_displacement(positions[i], positions[j]);
                auto const f = pot.evaluate_force(r);

                forces[i] += f;
                forces[j] -= f;
            }
        }

    public:
        Derived& derived() noexcept
        {
            return static_cast<Derived&>(*this);
        }
    };

    template<typename List, typename Box, typename PotFun>
    class basic_pairwise_forcefield
        : public md::pairwise_forcefield<basic_pairwise_forcefield<List, Box, PotFun>>
    {
    public:
        basic_pairwise_forcefield(PotFun potfun)
            : potfun_{potfun}
        {
        }

        basic_pairwise_forcefield& set_pairs(List const& pairs)
        {
            pairs_ = pairs;
            return *this;
        }

        basic_pairwise_forcefield& set_box(Box const& box)
        {
            box_ = box;
            return *this;
        }

        List const& pair_list(md::system const&) const
        {
            return pairs_;
        }

        Box const& box(md::system const&) const
        {
            return box_;
        }

        auto pairwise_potential(md::system const&, md::index i, md::index j) const
        {
            return potfun_(i, j);
        }

    private:
        PotFun potfun_;
        List pairs_;
        Box box_;
    };

    template<typename List, typename Box, typename P>
    auto make_pairwise_forcefield(P pot, List const& pairs, Box const& box)
    {
        auto potfun = detail::make_pair_potfun(pot);
        using potfun_type = decltype(potfun);
        md::basic_pairwise_forcefield<List, Box, potfun_type> ff{potfun};
        ff.set_pairs(pairs);
        ff.set_box(box);
        return ff;
    }
}
