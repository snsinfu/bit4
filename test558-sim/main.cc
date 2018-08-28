#include <iostream>

#include <sim/all.hpp>


int main()
{
    sim::system system;

    for (sim::index i = 0; i < 10; i++) {
        system.add_particle({
            .mass = 1,
            .point = {0, 0, sim::scalar(i)}
        });
    }

    // XXX: Feels like these should use plural. view_points is good, but view_velocities, forces
    // and masses look a bit strange. Maybe view_point_array?
    auto const point = system.view_point();
    auto const velocity = system.view_velocity();
    auto const force = system.view_force();
    auto const mass = system.view_mass();

    sim::scalar const k = 1;
    sim::scalar const dt = 0.01;

    for (int step = 0; step < 10000000; step++) {
        // Symplectic Euler
        //
        // v(n) = v(n-1) + 1/m F(x(n-1)) dt
        // x(n) = x(n-1) + v(n) dt

        for (sim::index i = 0; i < system.particle_count(); i++) {
            force[i] = -k * point[i].vector();
        }

        for (sim::index i = 0; i < system.particle_count(); i++) {
            velocity[i] += dt * force[i] / mass[i];
        }

        for (sim::index i = 0; i < system.particle_count(); i++) {
            point[i] += dt * velocity[i];
        }
    }

    for (sim::index i = 0; i < system.particle_count(); i++) {
        std::cout << point[i] << '\t' << velocity[i] << '\n';
    }
}
