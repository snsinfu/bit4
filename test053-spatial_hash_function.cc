#include <iostream>
#include <random>
#include <unordered_map>

#include <cstddef>
#include <cstdint>

namespace
{
    std::size_t hash(std::uint64_t x, std::uint64_t y, std::uint64_t z, std::size_t n)
    {
        // https://stackoverflow.com/a/5929567

        std::uint64_t const p1 = 73856093;
        std::uint64_t const p2 = 19349669;
        std::uint64_t const p3 = 83492791;

        x *= p1;
        y *= p2;
        z *= p3;

        return (x + y + z) % n;
    }
}

int main()
{
    std::size_t const n = 4098;

    auto const show_delta = [=](std::uint64_t x, std::uint64_t y, std::uint64_t z) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dz = -1; dz <= 1; ++dz) {
                    std::size_t const a = hash(x + dx, y + dy, z + dz, n);
                    std::size_t const b = hash(x, y, z, n);
                    std::cout << (n + a - b) % n << '\t';
                }
            }
        }
        std::cout << '\n';
    };

    show_delta(10, 10, 10);
    show_delta(11, 12, 13);
}

void main_()
{
    std::size_t const n = 4098;

    std::mt19937_64 engine;

    std::uint64_t const offset = 1uLL << 50;
    std::uint64_t const range = 100;
    std::uniform_int_distribution<std::uint64_t> coord{offset, offset + range};

    std::unordered_map<std::size_t, int> histogram;

    for (std::uint32_t i = 0; i < 30000; ++i) {
        std::uint64_t const x = coord(engine);
        std::uint64_t const y = coord(engine);
        std::uint64_t const z = coord(engine);

        std::size_t const val = hash(x, y, z, n);
        histogram[val] += 1;
    }

    std::unordered_map<int, int> prob_dist;

    for (std::size_t val = 0; val < n; ++val) {
        prob_dist[histogram[val]] += 1;
    }

    for (auto const count_freq : prob_dist) {
        std::cout << count_freq.first
                  << '\t'
                  << count_freq.second
                  << '\n';
    }
}
