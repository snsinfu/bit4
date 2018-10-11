#include <iostream>
#include <memory>
#include <random>

#include "sim.hpp"


class particle_forcefield : public sim::pairwise_forcefield<particle_forcefield> {
    static constexpr sim::scalar cutoff_distance = 0.2;

public:
    sim::softcore_potential pairwise_potential(sim::system const&, sim::index, sim::index) const
    {
        return sim::softcore_potential {
            .repulsive_energy = 1,
            .reciprocal_cutoff_distance = 1 / cutoff_distance
        };
    }

    sim::scalar pairwise_cutoff(sim::system const&) const
    {
        return cutoff_distance;
    }
};


int main()
{
    sim::system system;

    for (int i = 0; i < 1000; i++) {
        system.add_particle();
    }

    std::mt19937_64 engine;
    std::uniform_real_distribution<sim::scalar> uniform{-0.01, 0.01};

    int i = 0;
    for (auto& pos : system.position_array()) {
        pos = {
            100 * uniform(engine),
            100 * uniform(engine),
            100 * uniform(engine)
        };
        i++;
    }

    system.add_forcefield(std::make_shared<particle_forcefield>());

    std::clog << system.compute_potential_energy() << '\n';

    sim::simulate_brownian_dynamics(system, {
        .timestep = 1e-6,
        .simulation_length = 10000,
        .random_seed = engine()
    });

    std::clog << system.compute_potential_energy() << '\n';
}
