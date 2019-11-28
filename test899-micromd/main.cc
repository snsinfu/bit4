#include <functional>
#include <memory>
#include <iostream>
#include <utility>
#include <vector>

#include <md.hpp>


//
// bonded_pairwise_forcefield
//

namespace mdx
{
    template<typename Derived>
    class bonded_pairwise_forcefield : public md::forcefield
    {
    public:
        md::scalar compute_energy(md::system const& system) override
        {
            md::scalar sum = 0;

            auto const& positions = system.view_positions();
            auto const& pairs = derived().bonded_pairs(system);

            for (auto const& pair : pairs) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const r = positions[i] - positions[j];
                auto const potential = derived().bonded_pairwise_potential(system, i, j);
                auto const energy = potential.evaluate_energy(r);
                sum += energy;
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const& positions = system.view_positions();
            auto const& pairs = derived().bonded_pairs(system);

            for (auto const& pair : pairs) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const r = positions[i] - positions[j];
                auto const potential = derived().bonded_pairwise_potential(system, i, j);
                auto const force = potential.evaluate_force(r);
                forces[i] += force;
                forces[j] -= force;
            }
        }

        //
        // CRTP callback stubs
        //

        inline md::array_view<std::pair<md::index, md::index> const>
        bonded_pairs(md::system const&) const
        {
            return {};
        }

        inline md::constant_potential
        bonded_pairwise_potential(md::system const&, md::index, md::index) const
        {
            return {};
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }
    };


    template<typename PotFun>
    class basic_bonded_pairwise_forcefield
        : public mdx::bonded_pairwise_forcefield<basic_bonded_pairwise_forcefield<PotFun>>
    {
    public:
        explicit basic_bonded_pairwise_forcefield(PotFun const& potfun)
            : _potfun{potfun}
        {
        }

        basic_bonded_pairwise_forcefield& add_bonded_pair(md::index i, md::index j)
        {
            _pairs.emplace_back(i, j);
            return *this;
        }

        //
        // CRTP callback implementations
        //

        inline std::vector<std::pair<md::index, md::index>> const&
        bonded_pairs(md::system const&) const
        {
            return _pairs;
        }

        inline auto
        bonded_pairwise_potential(md::system const&, md::index i, md::index j) const
        {
            return _potfun(i, j);
        }

    private:
        std::vector<std::pair<md::index, md::index>> _pairs;
        PotFun _potfun;
    };


    template<typename P>
    auto make_bonded_pairwise_forcefield(P const& pot)
    {
        auto potfun = md::detail::make_pair_potfun(pot);
        using potfun_type = decltype(potfun);
        return mdx::basic_bonded_pairwise_forcefield<potfun_type>{potfun};
    }
}


//
// neighbor_pairwise_forcefield
//

namespace mdx
{
    template<typename Derived, typename Box = md::open_box>
    class neighbor_pairwise_forcefield : public md::forcefield
    {
    public:
        md::scalar compute_energy(md::system const& system) override
        {
            md::scalar sum = 0;

            auto const& positions = system.view_positions();
            auto const& neighbor_pairs = compute_neighbor_list(system);

            for (auto const& pair : neighbor_pairs) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const r = positions[i] - positions[j];
                auto const potential = derived().neighbor_pairwise_potential(system, i, j);
                auto const energy = potential.evaluate_energy(r);
                sum += energy;
            }

            return sum;
        }

        void compute_force(md::system const& system, md::array_view<md::vector> forces) override
        {
            auto const& positions = system.view_positions();
            auto const& neighbor_pairs = compute_neighbor_list(system);

            for (auto const& pair : neighbor_pairs) {
                auto const i = pair.first;
                auto const j = pair.second;
                auto const r = positions[i] - positions[j];
                auto const potential = derived().neighbor_pairwise_potential(system, i, j);
                auto const force = potential.evaluate_force(r);
                forces[i] += force;
                forces[j] -= force;
            }
        }

        //
        // CRTP callback stubs
        //

        inline Box bounding_box(md::system const&) const
        {
            return {};
        }

        inline md::scalar neighbor_distance(md::system const&) const
        {
            return 0;
        }

        inline md::constant_potential
        neighbor_pairwise_potential(md::system const&, md::index, md::index) const
        {
            return {};
        }

    private:
        inline Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        md::neighbor_list_v2<Box> compute_neighbor_list(md::system const& system)
        {
            _neighbor_list.update(
                system.view_positions(),
                derived().neighbor_distance(system),
                derived().bounding_box(system)
            );
            return _neighbor_list;
        }

    private:
        md::neighbor_list_v2<Box> _neighbor_list;
    };


    template<typename PotFun, typename Box = md::open_box>
    class basic_neighbor_pairwise_forcefield
        : public mdx::neighbor_pairwise_forcefield<basic_neighbor_pairwise_forcefield<PotFun>, Box>
    {
    public:
        using box_type = Box;

        explicit basic_neighbor_pairwise_forcefield(PotFun const& potfun)
            : _potfun{potfun}
        {
            _box_callback = [] { return box_type{}; };
            _distance_callback = [] { return 0; };
        }

        basic_neighbor_pairwise_forcefield& set_bounding_box(box_type const& box)
        {
            return set_bounding_box([=] { return box; });
        }

        basic_neighbor_pairwise_forcefield& set_bounding_box(std::function<box_type()> box_cb)
        {
            _box_callback = box_cb;
            return *this;
        }

        basic_neighbor_pairwise_forcefield& set_neighbor_distance(md::scalar dist)
        {
            return set_neighbor_distance([=] { return dist; });
        }

        basic_neighbor_pairwise_forcefield& set_neighbor_distance(std::function<md::scalar()> dist_cb)
        {
            _distance_callback = dist_cb;
            return *this;
        }

        //
        // CRTP callback implementations
        //

        inline box_type
        bounding_box(md::system const&) const
        {
            return _box_callback();
        }

        inline md::scalar
        neighbor_distance(md::system const&) const
        {
            return _distance_callback();
        }

        inline auto
        neighbor_pairwise_potential(md::system const&, md::index i, md::index j) const
        {
            return _potfun(i, j);
        }

    private:
        PotFun _potfun;
        std::function<box_type()> _box_callback;
        std::function<md::scalar()> _distance_callback;
    };


    template<typename Box = md::open_box, typename P>
    auto make_neighbor_pairwise_forcefield(P const& pot)
    {
        auto potfun = md::detail::make_pair_potfun(pot);
        using potfun_type = decltype(potfun);
        return mdx::basic_neighbor_pairwise_forcefield<potfun_type, Box>{potfun};
    }
}



int main()
{
    md::system system;

    system.add_particle();
    system.add_particle();
    system.add_particle();
    system.add_particle();

    system.add_forcefield(
        mdx::make_neighbor_pairwise_forcefield(
            md::polybell_potential<2, 3> {
                .overlap_energy  = 3,
                .cutoff_distance = 0.1
            }
        )
        .set_neighbor_distance([&] {
            return 0.1;
        })
    );

    system.add_forcefield(
        mdx::make_bonded_pairwise_forcefield(
            md::spring_potential {
                .spring_constant      = 1000,
                .equilibrium_distance = 0.1
            }
        )
        .add_bonded_pair(0, 1)
        .add_bonded_pair(1, 2)
        .add_bonded_pair(2, 3)
        .add_bonded_pair(3, 0)
    );

    md::simulate_brownian_dynamics(system, {
        .temperature = 1,
        .timestep    = 1e-5,
        .steps       = 10000
    });

    for (auto pos : system.view_positions()) {
        std::cout << pos << '\n';
    }
}
