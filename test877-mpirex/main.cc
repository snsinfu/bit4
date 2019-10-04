#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "rex/replica_agent.hpp"


int main(int argc, char** argv)
{
    rex::this_process this_process{argc, argv};

    auto const rank = rex::rank(this_process.world());
    auto const replica_count = rex::size(this_process.world());
    std::mt19937_64 random{rank};

    std::vector<double> temperatures = { 1 };
    for (int i = 1; i < replica_count; i++) {
        temperatures.push_back(temperatures.back() * 1.1);
    }

    rex::replica_agent replica{this_process.world(), 0, temperatures};

    for (int i = 0; i < 10; i++) {
        std::exponential_distribution<double> energy_distr{1 / replica.temperature()};
        auto energy = energy_distr(random);
        for (int i = 0; i < 10'000'000; i++) {
            energy = 0.999 * energy + 0.001 * energy_distr(random);
        }

        auto const exchanged = replica.exchange(energy, random);

        std::cout
            << i
            << '\t'
            << replica.id()
            << '\t'
            << (exchanged ? "x" : "=")
            << '\t'
            << energy
            << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds{10});
}
