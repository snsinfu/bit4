#include <iostream>

#include <md.hpp>


int main()
{
    md::system system;
    md::random_engine random;

    for (int i = 0; i < 1000; i++) {
        std::uniform_real_distribution<md::scalar> x{0, 3};
        std::uniform_real_distribution<md::scalar> y{0, 1};
        std::uniform_real_distribution<md::scalar> z{-1, 1};
        system.add_particle({
            .mobility = 1,
            .position = {x(random), y(random), z(random)}
        });
    }

    system.add_forcefield(
        md::make_neighbor_pair_forcefield_v2<md::xy_periodic_box>(
            md::polybell_potential<5, 3>{
                .overlap_energy  = 3,
                .cutoff_distance = 0.1
            }
        )
        .set_box(md::xy_periodic_box{
            .x_period = 3,
            .y_period = 1
        })
        .set_neighbor_distance(0.1)
    );

    md::simulate_brownian_dynamics(system, {
        .timestep = 1e-5,
        .steps    = 10000,
        .callback = [&](md::step step) {
            if (step % 1000 == 0) {
                std::clog << step << '\n';
            }
        }
    });
}
