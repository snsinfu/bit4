// system - collection of particles, their properties and forcefield

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SIM_SYSTEM_HPP
#define INCLUDED_SIM_SYSTEM_HPP

#include "forcefield.hpp"
#include "typedef.hpp"


namespace sim
{
    class system
    {
    public:
        void add_particle();
        sim::index particle_count() const noexcept;

        template<typename T>
        sim::array_view<T> require_property_array();

        template<typename T>
        sim::array_view<T> property_array();

        template<typename T>
        sim::array_view<T> property_array() const;

        sim::array_view<sim::scalar> mass_array() noexcept;
        sim::array_view<sim::scalar const> mass_array() const noexcept;

        sim::array_view<sim::point> position_array() noexcept;
        sim::array_view<sim::point const> position_array() const noexcept;

        sim::array_view<sim::vector> velocity_array() noexcept;
        sim::array_view<sim::vector const> velocity_array() const noexcept;

        void add_forcefield(std::shared_ptr<sim::forcefield> forcefield);
        sim::scalar compute_energy();
        void compute_force(sim::array_view<sim::vector> forces);
    };
}

#endif
