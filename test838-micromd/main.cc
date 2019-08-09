#include <iostream>
#include <random>

#include <md.hpp>


md::attribute_key<md::scalar, struct radius_attribute_key> radius_attribute;


struct my_forcefield
    : md::composite_forcefield<
        md::all_pair_forcefield      <my_forcefield>,
        md::sphere_surface_forcefield<my_forcefield>
    >
{
    auto all_pair_potential(md::system const& system, md::index i, md::index j) const
    {
        auto const radius = system.view(radius_attribute);

        return md::soft_lennard_jones_potential {
            .epsilon  = 1,
            .sigma    = radius[i] + radius[j],
            .softness = 0.1,
        };
    }

    auto sphere_outward_potential(md::system const&, md::index) const
    {
        return md::harmonic_potential { .spring_constant = 100 };
    }
};


int main()
{
    std::mt19937_64 random;
    md::system system;

    system.require(radius_attribute);

    for (int i = 0; i < 100; i++) {
        std::uniform_real_distribution<md::scalar> coord{-1, 1};
        auto part = system.add_particle({
            .mobility = 1,
            .position = {coord(random), coord(random), coord(random)},
        });

        std::uniform_real_distribution<md::scalar> radius{0.5, 1.5};
        part.view(radius_attribute) = radius(random);
    }

    system.add_forcefield(
        my_forcefield{}
        .set_sphere(md::sphere { .radius = 1 })
    );
}
