#include <iostream>
#include <random>
#include <memory>

#include "sim.hpp"


class my_forcefield : public sim::sequential_pair_forcefield<my_forcefield>
{
public:
    sim::harmonic_potential sequential_pair_potential(sim::system const&, sim::index, sim::index)
    {
        return sim::harmonic_potential {
            .spring_constant = 1,
        };
    }
};


int main()
{
    sim::system system;

    for (int i = 0; i < 100; i++) {
        system.add_particle();
    }

    std::mt19937_64 engine;
    std::uniform_real_distribution<sim::scalar> uniform{-1, 1};

    for (auto& pos : system.view_position_array()) {
        pos = {uniform(engine), uniform(engine), uniform(engine)};
    }

    auto ff = std::make_shared<my_forcefield>();
    ff->add_segment(0, 49);
    ff->add_segment(50, 99);
    system.add_forcefield(ff);

    std::cout << system.compute_potential_energy() << '\n';
}
