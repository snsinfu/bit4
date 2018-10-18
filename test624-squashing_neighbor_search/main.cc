#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include "bruteforce_neighbor_searcher.hpp"
#include "hashing_neighbor_searcher.hpp"
#include "squashing_neighbor_searcher.hpp"
#include "finehashing_neighbor_searcher.hpp"
#include "overlapped_hashing_neighbor_searcher.hpp"


constexpr md::scalar cutoff_distance = 0.04;
constexpr md::index point_count = 100000;
constexpr md::index hash_bin_count = point_count / 16;
constexpr int bench_count = 100;


md::index hash_pairs(md::array_view<std::pair<md::index, md::index> const> pairs)
{
    md::index hash = 0;

    for (auto pair : pairs) {
        hash += ~((pair.first - pair.second) * (pair.second - pair.first));
    }

    return hash;
}


template<typename Searcher>
void benchmark(Searcher& searcher)
{
    using clock = std::chrono::steady_clock;

    std::mt19937_64 random;
    std::uniform_real_distribution<md::scalar> coord{-1, 1};

    std::vector<md::point> points;
    std::vector<std::pair<md::index, md::index>> pairs;
    md::index hash_sum = 0;

    auto const start_time = clock::now();

    for (int iter = 0; iter < bench_count; iter++) {
        points.clear();
        std::generate_n(std::back_inserter(points), point_count, [&] {
            return md::point {coord(random), coord(random), coord(random)};
        });

        pairs.clear();
        searcher.set_points(points);
        searcher.search(std::back_inserter(pairs));

        std::sort(pairs.begin(), pairs.end());
        pairs.erase(std::unique(pairs.begin(), pairs.end()), pairs.end());

        hash_sum += hash_pairs(pairs);
    }

    auto const end_time = clock::now();
    auto const duration = std::chrono::duration_cast<std::chrono::duration<double>>(
        end_time - start_time
    );
    double const mean_duration_msec = 1e3 * duration.count() / bench_count;

    std::cout << "Hash: 0x" << std::hex << hash_sum << std::dec << '\n';
    std::cout << "Time: " << mean_duration_msec << " ms\n";
}


int main()
{
    md::hashing_neighbor_searcher hashing{cutoff_distance, hash_bin_count};
    md::finehashing_neighbor_searcher finehashing{cutoff_distance, hash_bin_count * 3};
    md::overlapped_hashing_neighbor_searcher overlapped_hashing{cutoff_distance, point_count};
    md::bruteforce_neighbor_searcher bruteforce{cutoff_distance};

    std::cout << "--- hashing_neighbor_searcher ---\n";
    benchmark(hashing);

    std::cout << "--- finehashing_neighbor_searcher ---\n";
    benchmark(finehashing);

    std::cout << "--- overlapped_hashing_neighbor_searcher ---\n";
    benchmark(overlapped_hashing);

    std::cout << "--- bruteforce_neighbor_searcher ---\n";
    benchmark(bruteforce);
}
