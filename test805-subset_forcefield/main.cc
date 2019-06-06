#include <chrono>
#include <iostream>
#include <random>

#include <md.hpp>

#include "subsystem_pair_forcefield.hpp"


int main()
{
    std::mt19937_64 random;
    md::system system;

    for (int i = 0; i < 1000; i++) {
        std::uniform_real_distribution<md::scalar> uniform{-1, 1};

        system.add_particle({
            .position = {
                uniform(random),
                uniform(random),
                uniform(random)
            }
        });
    }

#if !defined(USE_ADHOC) || !USE_ADHOC
    auto sub_forces = system.add_forcefield(
        make_subsystem_pair_forcefield(
            md::soft_lennard_jones_potential{
                .epsilon  = 1,
                .sigma    = 0.02,
                .softness = 0.1,
            }
        )
    );
    for (md::index i = 0; i < system.particle_count(); i++) {
        if (i % 3 == 0) {
            sub_forces->add_target(i);
        }
    }
#else
    auto sub_forces = system.add_forcefield(
        md::make_all_pair_forcefield(
            [](md::index i, md::index j) {
                if (i % 3 || j % 3) {
                    return md::soft_lennard_jones_potential{.epsilon = 0};
                }
                return md::soft_lennard_jones_potential{
                    .epsilon  = 1,
                    .sigma    = 0.02,
                    .softness = 0.1,
                };
            }
        )
    );
#endif

    using clock = std::chrono::steady_clock;
    const auto start_time = clock::now();

    md::simulate_brownian_dynamics(system, {
        .timestep = 1e-5,
        .steps    = 100000
    });
    const auto mean_energy = system.compute_energy() / system.particle_count();

    const auto finish_time = clock::now();
    const auto elapsed_time = finish_time - start_time;
    const auto elapsed_time_sec = std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();

    std::cout << elapsed_time_sec << '\t' << mean_energy << '\n';
}
