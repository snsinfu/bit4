#include <algorithm>
#include <iostream>
#include <random>

#include "md.hpp"


int main()
{
    md::system system;

    std::mt19937_64 random;
    std::uniform_real_distribution<md::scalar> coord{-1, 1};

    for (md::index i = 0; i < 1000; i++) {
        system.add_particle({
            .position = {coord(random), coord(random), coord(random)}
        });
    }

    md::force_neighbor_pairs(system, 0.1, md::softcore_potential {
        .overlap_energy = 3.0,
        .cutoff_distance = 0.1
    });

    md::force_sphere_surface(system, 1, md::harmonic_potential {
        .spring_constant = 1000
    });

    md::simulate_brownian_dynamics(system, {
        .timestep = 1.0e-4,
        .spacestep = 0.001,
        .steps = 10'000,
        .seed = random()
    });
}
