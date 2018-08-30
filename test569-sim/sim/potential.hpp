// Potential energy classes

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SIM_POTENTIAL_HPP
#define INCLUDED_SIM_POTENTIAL_HPP

#include "typedef.hpp"


namespace sim
{
    // harmonic_potential
    struct harmonic_potential
    {
        sim::scalar spring_constant = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            return 0.5 * spring_constant * r.squared_norm();
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            return -spring_constant * r;
        }
    };

    // lennard_jones_potential
    struct lennard_jones_potential
    {
        sim::scalar epsilon = 1;
        sim::scalar sigma = 1;

        inline sim::scalar evaluate_energy(sim::vector r) const
        {
            auto const u2 = sigma * sigma / r.squared_norm();
            auto const u6 = u2 * u2 * u2;

            return epsilon * (u6 * u6 - (u6 + u6));
        }

        inline sim::vector evaluate_force(sim::vector r) const
        {
            auto const r_inv = 1 / r.squared_norm();
            auto const u2 = sigma * sigma * r_inv;
            auto const u6 = u2 * u2 * u2;

            return (12 * epsilon) * ((u6 * u6 - u6) * r_inv) * r;
        }
    };
}

#endif
