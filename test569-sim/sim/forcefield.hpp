// Forcefield

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SIM_FORCEFIELD_HPP
#define INCLUDED_SIM_FORCEFIELD_HPP

#include <memory>
#include <vector>

#include "typedef.hpp"


namespace sim
{
    class system;

    class forcefield
    {
    public:
        virtual ~forcefield() = default;

        virtual sim::scalar compute_energy(sim::system const& system) = 0;

        virtual void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) = 0;
    };

    class composite_forcefield : public sim::forcefield
    {
    public:
        void add_component(std::shared_ptr<sim::forcefield> forcefield)
        {
            components_.push_back(forcefield);
        }

        sim::scalar compute_energy(sim::system const& system) override
        {
            sim::scalar total_energy = 0;
            for (auto& component : components_) {
                total_energy += component->compute_energy(system);
            }
            return total_energy;
        }

        void compute_force(sim::system const& system, sim::array_view<sim::vector> forces) override
        {
            for (auto& component : components_) {
                component->compute_force(system, forces);
            }
        }

    private:
        std::vector<std::shared_ptr<sim::forcefield>> components_;
    };
}

#endif
