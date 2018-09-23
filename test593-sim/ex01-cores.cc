#include <iostream>
#include <memory>
#include <vector>

#include "sim.hpp"


class my_forcefield : public sim::composite_forcefield<sim::pairwise_forcefield<my_forcefield>>
{
public:
    auto pairwise_potential(sim::system const&, sim::index, sim::index) const
    {
        return sim::softcore_lennard_jones_potential{
            .epsilon = 1.0,
            .sigma = 0.1,
            .softness = 0.2
        };
    }
};


int main()
{
    sim::system system;

    for (int i = 0; i < 10000; i++) {
        system.add_particle();
    }

    system.add_forcefield(std::make_shared<my_forcefield>());

    std::cout << system.compute_potential_energy() << '\n';
}
