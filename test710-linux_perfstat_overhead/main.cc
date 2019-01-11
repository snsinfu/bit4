// Benchmark task: Lennard-Jones fluid simulation.

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>


struct vec3
{
    double x = 0;
    double y = 0;
    double z = 0;
};

vec3 operator-(vec3 const& v)
{
    return vec3{-v.x, -v.y, -v.z};
}

vec3 operator+(vec3 const& v1, vec3 const& v2)
{
    return vec3{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3 operator-(vec3 const& v1, vec3 const& v2)
{
    return v1 + (-v2);
}

vec3 operator*(double a, vec3 const& v)
{
    return vec3{a * v.x, a * v.y, a * v.z};
}

double squared_norm(vec3 const& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}


int main()
{
    std::size_t const particle_count = 10000;
    long const steps = 100;
    double const dt = 0.00001;

    std::mt19937_64 random;

    std::vector<double> masses(particle_count, 1);
    std::vector<double> epsilons(particle_count, 1);
    std::vector<double> sigmas(particle_count, 0.02);
    std::vector<vec3> positions(particle_count);
    std::vector<vec3> velocities(particle_count);
    std::vector<vec3> forces(particle_count);

    // Initialization

    std::uniform_real_distribution<double> uniform;
    std::generate(positions.begin(), positions.end(), [&] {
        return vec3{uniform(random), uniform(random), uniform(random)};
    });

    std::normal_distribution<double> normal;
    std::generate(velocities.begin(), velocities.end(), [&] {
        return vec3{normal(random), normal(random), normal(random)};
    });

    // Forcefield

    auto const compute_forcefield = [&] {
        double total_energy = 0;
        std::fill(forces.begin(), forces.end(), vec3{0, 0, 0});

        for (std::size_t i = 0; i < particle_count; i++) {
            for (std::size_t j = 0; j < i; j++) {
                auto const epsilon = std::sqrt(epsilons[i] * epsilons[j]);
                auto const sigma = sigmas[i] + sigmas[j];
                auto const sigma2 = sigma * sigma;

                auto const r = positions[i] - positions[j];
                auto const r2 = squared_norm(r);
                auto const r2_inv = 1 / r2;
                auto const u2 = sigma2 * r2_inv;
                auto const u6 = u2 * u2 * u2;

                auto const force = 12 * epsilon * (u6 * u6 - u6) * r2_inv * r;
                auto const energy = epsilon * (u6 * u6 - u6 - u6);

                forces[i] = forces[i] + force;
                forces[j] = forces[j] - force;

                total_energy += energy;
            }
        }

        return total_energy;
    };

    // Velocity Verlet integration

    for (long step = 1; step <= steps; step++) {
        for (std::size_t i = 0; i < particle_count; i++) {
            velocities[i] = velocities[i] + dt / (2 * masses[i]) * forces[i];
            positions[i] = positions[i] + dt * velocities[i];
        }

        compute_forcefield();

        for (std::size_t i = 0; i < particle_count; i++) {
            velocities[i] = velocities[i] + dt / (2 * masses[i]) * forces[i];
        }
    }

    std::cout << compute_forcefield() << '\n';
}
