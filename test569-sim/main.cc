#include <iostream>
#include <vector>

#include <sim/potential.hpp>


int main()
{
    auto pot = sim::harmonic_potential({
        .spring_constant = 100
    });
    auto r = sim::vector {1, 2, 3};

    std::cout << pot.evaluate_energy(r) << '\n';
}
