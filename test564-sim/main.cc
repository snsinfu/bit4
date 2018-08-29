#include <cmath>
#include <iostream>
#include <random>

#include <sim/all.hpp>


void simulate_brownian_dynamics(sim::system& system, std::mt19937& random_engine)
{
    sim::scalar const timestep = 0.01;
    sim::scalar const temperature = 1;

    auto const point_array = system.view_point_array();
    auto const force_array = system.view_force_array();
    auto const mobility_array = system.view_mobility_array();

    std::vector<sim::vector> previous_weiner(system.point_count());
    sim::normal_distribution<sim::vector> normal;

    for (int step = 0; step < 1000000; step++) {

        for (auto& force : force_array) {
            force = {};
        }
        system.compute_force();

        for (sim::index i = 0; i < system.particle_count(); i++) {
            auto const drift = mobility_array[i] * timestep;
            auto const sigma = std::sqrt(2 * drift * temperature * timestep);
            auto const weiner = sigma * normal(random_engine);

            point_array[i] += drift * force_array[i] + 0.5 * (weiner + previous_weiner[i]);
            previous_weiner[i] = weiner;
        }

        system.set_time(system.time() + timestep);
    }
}

class my_forcefield : public sim::forcefield
{
    void compute_energy(sim::system& system, sim::scalar& energy) override
    {
    }

    void compute_force(sim::system& system, sim::array_view<sim::vector> force_array) override
    {
    }
};

int main()
{
    std::mt19937 random_engine;
    sim::system system;

    for (sim::index i = 0; i < 10; i++) {
        system.add_particle({
            .mass = 1,
            .point = {0, 0, i / 9.0}
        });
    }

    system.add_forcefield(std::make_shared<my_forcefield>());

    simulate_brownian_dynamics(system, random_engine);

    for (sim::index i = 0; i < system.particle_count(); i++) {
        std::cout << point[i] << '\t' << velocity[i] << '\n';
    }
}
