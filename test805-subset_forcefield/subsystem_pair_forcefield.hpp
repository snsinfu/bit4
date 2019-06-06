#pragma once

#include <vector>

#include <md.hpp>


template<typename Derived>
class subsystem_pair_forcefield : public virtual md::forcefield
{
public:
    void add_target(md::index i)
    {
        _targets.push_back(i);
    }

    md::scalar compute_energy(const md::system& system) override
    {
        std::vector<particle_eval> part_evals = _make_part_evals(system);
        md::scalar energy = 0;

        for (md::index i = 0; i < part_evals.size(); i++) {
            for (md::index j = i + 1; j < part_evals.size(); j++) {
                auto& part_i = part_evals[i];
                auto& part_j = part_evals[j];

                auto const pot = _derived().subsystem_pair_potential(
                    system, part_i.index, part_j.index
                );
                auto const r = part_i.position - part_j.position;

                energy += pot.evaluate_energy(r);
            }
        }

        return energy;
    }

    void compute_force(const md::system& system, md::array_view<md::vector> forces) override
    {
        std::vector<particle_eval> part_evals = _make_part_evals(system);

        for (md::index i = 0; i < part_evals.size(); i++) {
            for (md::index j = i + 1; j < part_evals.size(); j++) {
                auto& part_i = part_evals[i];
                auto& part_j = part_evals[j];

                auto const pot = _derived().subsystem_pair_potential(
                    system, part_i.index, part_j.index
                );
                auto const r = part_i.position - part_j.position;

                auto const force = pot.evaluate_force(r);
                part_i.force += force;
                part_j.force -= force;
            }
        }

        for (auto part : part_evals) {
            forces[part.index] += part.force;
        }
    }

private:
    struct particle_eval
    {
        md::index  index    = 0;
        md::point  position = {};
        md::vector force    = {};
    };

    std::vector<particle_eval> _make_part_evals(const md::system& system) const
    {
        const auto positions = system.view_positions();

        std::vector<particle_eval> part_evals;
        part_evals.reserve(_targets.size());

        for (auto idx : _targets) {
            particle_eval part;
            part.index = idx;
            part.position = positions[idx];
            part_evals.push_back(part);
        }

        return part_evals;
    }

    Derived& _derived()
    {
        return static_cast<Derived&>(*this);
    }

    std::vector<md::index> _targets;
};


template<typename PotFun>
class basic_subsystem_pair_forcefield
    : public subsystem_pair_forcefield<basic_subsystem_pair_forcefield<PotFun>>
{
public:
    explicit basic_subsystem_pair_forcefield(PotFun potfun)
        : potfun_{potfun}
    {
    }

    auto subsystem_pair_potential(md::system const&, md::index i, md::index j) const
    {
        return potfun_(i, j);
    }

private:
    PotFun potfun_;
};

template<typename P>
auto make_subsystem_pair_forcefield(P pot)
{
    auto potfun = md::detail::make_pair_potfun(pot);
    using potfun_type = decltype(potfun);
    return basic_subsystem_pair_forcefield<potfun_type>{potfun};
}
