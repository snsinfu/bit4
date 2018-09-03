#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

#include "geo.hpp"


using scalar_t = double;
using vector_t = geo::vector<scalar_t, 3>;

// solve_for_timestep returns a value of dt satisfying the equation of motion (1) given constraint
// |dr| <= displacement.
//
//     dr = velocity * dt + force * dt^2 / (2 * mass)    (1)
//
scalar_t solve_for_timestep(
    scalar_t displacement,
    vector_t velocity,
    vector_t force,
    scalar_t mass
)
{
    scalar_t const vF = velocity.dot(force);
    scalar_t const mv = mass * velocity.norm();
    scalar_t const K = 0.5 * mass * velocity.squared_norm();

    // Initial estimate
    scalar_t dt = (std::sqrt(K * K + mv * vF * displacement) - K) / vF;

    if (!(dt > 0)) {
        dt = 0;
    }

    for (int i = 0; i < 100; i++) {
        dt = 0.7 * dt + 0.3 * displacement / norm(velocity + force * dt / (2 * mass));
    }

    return dt;
}

// compute_displacement returns the displacement vector dr given by (1).
//
//     dr = velocity * timestep + force * timestep^2 / (2 * mass)    (1)
//
vector_t compute_displacement(scalar_t timestep, vector_t velocity, vector_t force, scalar_t mass)
{
    return timestep * (velocity + timestep / (2 * mass) * force);
}

int main()
{
    std::mt19937 engine;
    {
        std::random_device source;
        std::seed_seq seed{source(), source(), source(), source()};
        engine.seed(seed);
    }

    std::uniform_real_distribution<double> uniform{-1, 1};

    std::cout << "dt\tdq\n";

    for (int i = 0; i < 10000; i++) {
        vector_t const v = {uniform(engine), uniform(engine), uniform(engine)};
        vector_t const F = {uniform(engine), uniform(engine), uniform(engine)};
        scalar_t const m = 1;
        scalar_t const D = 0.1;

        auto const dt = solve_for_timestep(D, v, F, m);
        auto const r = compute_displacement(dt, v, F, m);

        std::cout << dt << '\t' << r.norm() << '\n';
    }

}
