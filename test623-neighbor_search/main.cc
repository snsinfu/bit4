#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "grid_neighbor_searcher.hpp"
#include "hashing_neighbor_searcher.hpp"


constexpr md::scalar cutoff_distance = 0.05;
constexpr md::index point_count = 10000;
constexpr md::index hash_bin_count = 3000;
constexpr int bench_count = 100;


md::index hash_pairs(std::vector<std::pair<md::index, md::index>> const& pairs)
{
    md::index hash = 0;

    for (auto pair : pairs) {
        hash += pair.first * pair.second;
    }

    return hash;
}


void benchmark_hashing_neighbor_searcher()
{
    std::mt19937_64 random;
    std::uniform_real_distribution<md::scalar> coord{-1, 1};

    std::vector<md::point> points;
    std::vector<std::pair<md::index, md::index>> pairs;
    md::index hash_sum = 0;

    using clock = std::chrono::steady_clock;

    auto const start_time = clock::now();

    md::hashing_neighbor_searcher searcher{cutoff_distance, hash_bin_count};

    for (int iter = 0; iter < bench_count; iter++) {
        points.clear();

        std::generate_n(std::back_inserter(points), point_count, [&] {
            return md::point {coord(random), coord(random), coord(random)};
        });

        pairs.clear();

        searcher.set_points(points);
        searcher.search(std::back_inserter(pairs));

        hash_sum += hash_pairs(pairs);
    }

    auto const end_time = clock::now();
    auto const duration = std::chrono::duration_cast<std::chrono::duration<double>>(
        end_time - start_time
    );
    auto const mean_duration_usec = 1e6 * duration.count() / bench_count;

    std::cout << "--- hashing_neighbor_searcher ---\n";
    std::cout << "Hash: 0x" << std::hex << hash_sum << '\n';
    std::cout << "Time: " << mean_duration_usec << " us\n";
}


void benchmark_grid_neighbor_searcher()
{
    std::mt19937_64 random;
    std::uniform_real_distribution<md::scalar> coord{-1, 1};

    std::vector<md::point> points;
    std::vector<std::pair<md::index, md::index>> pairs;
    md::index hash_sum = 0;

    using clock = std::chrono::steady_clock;

    auto const start_time = clock::now();

    md::grid_neighbor_searcher searcher{cutoff_distance};

    for (int iter = 0; iter < bench_count; iter++) {
        points.clear();

        std::generate_n(std::back_inserter(points), point_count, [&] {
            return md::point {coord(random), coord(random), coord(random)};
        });

        pairs.clear();

        searcher.set_points(points);
        searcher.search(std::back_inserter(pairs));

        hash_sum += hash_pairs(pairs);
    }

    auto const end_time = clock::now();
    auto const duration = std::chrono::duration_cast<std::chrono::duration<double>>(
        end_time - start_time
    );
    auto const mean_duration_usec = 1e6 * duration.count() / bench_count;

    std::cout << "--- grid_neighbor_searcher ---\n";
    std::cout << "Hash: 0x" << std::hex << hash_sum << '\n';
    std::cout << "Time: " << mean_duration_usec << " us\n";
}


int main()
{
    benchmark_hashing_neighbor_searcher();
    benchmark_grid_neighbor_searcher();
}
