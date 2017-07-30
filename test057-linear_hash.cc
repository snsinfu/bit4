#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <cmath>
#include <cstddef>


int main()
{
    std::size_t const px = 73856093;
    std::size_t const py = 19349669;
    std::size_t const pz = 83492791;
    std::size_t const modulo = 110;

    bool const use_xor = false;
    bool const use_random = true;

    auto const hash = [=](std::size_t x, std::size_t y, std::size_t z) {
        if (use_xor) {
            return ((px * x) ^ (py * y) ^ (pz * z)) % modulo;
        } else {
            return (px * x + py * y + pz * z) % modulo;
        }
    };

    std::size_t const max_x = 50;
    std::size_t const max_y = 50;
    std::size_t const max_z = 50;

    std::vector<int> histogram(modulo);
    std::size_t total_count = 0;

    if (!use_random) {
        for (std::size_t x = 0; x < max_x; ++x) {
            for (std::size_t y = 0; y < max_y; ++y) {
                for (std::size_t z = 0; z < max_z; ++z) {
                    std::size_t const h = hash(x, y, z);
                    histogram[h] += 1;
                    total_count += 1;
                }
            }
        }
    } else {
        total_count = modulo * 50;
        std::mt19937_64 engine;

        for (std::size_t i = 0; i < total_count; ++i) {
            std::uniform_int_distribution<std::size_t> x_dist{0, max_x};
            std::uniform_int_distribution<std::size_t> y_dist{0, max_y};
            std::uniform_int_distribution<std::size_t> z_dist{0, max_z};
            std::size_t const x = x_dist(engine);
            std::size_t const y = y_dist(engine);
            std::size_t const z = z_dist(engine);
            std::size_t const h = hash(x, y, z);
            histogram[h] += 1;
        }
    }

    std::size_t const scale = total_count / 20;

    for (std::size_t h = 0; h < modulo; ++h) {
        std::cout << h
                  << '\t'
                  << histogram[h]
                  << '\t'
                  << std::string(histogram[h] * modulo / scale, '.')
                  << '\n';
    }
}
