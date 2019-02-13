#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include <point.hpp>

#include "nsearch.hpp"


struct pair_hash
{
    uint32_t hash = 0;
    uint32_t count = 0;

    void update(uint32_t i, uint32_t j)
    {
        hash += (~i * j) ^ (i * ~j);
        count++;
    }
};


template<typename Op>
double timeit(Op op)
{
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();
    op();
    const auto end = clock::now();
    const auto time = end - start;
    return std::chrono::duration_cast<std::chrono::duration<double>>(time).count();
}


int main()
{
    using std::uint32_t;

    const uint32_t n_points = 4000;
    const cxx::vector period = {4.5, 1.5, 0};
    const double dcut = 0.3;

    std::vector<cxx::point> points;
    std::mt19937 random;
    std::generate_n(std::back_inserter(points), n_points, [&] {
        std::uniform_real_distribution<double> coord{0, 13.5};
        return cxx::point{coord(random), coord(random), coord(random)};
    });

    // Neighbor search using xy_periodic_neighbor_searcher

    pair_hash searcher_hash;

    const auto searcher_time = timeit([&] {
        xy_periodic_neighbor_searcher searcher{points, dcut, period};
        searcher.foreach_neighbor([&](uint32_t i, uint32_t j) {
            searcher_hash.update(i, j);
        });
    });

    std::cout
        << "Searcher:\t"
        << searcher_time << '\t'
        << searcher_hash.count << '\t'
        << searcher_hash.hash << '\n';

    // Neighbor search using xy_periodic_neighbor_list

    pair_hash list_hash;

    const auto list_time = timeit([&] {
        xy_periodic_neighbor_list list{dcut, period};
        list.update(points);
        for (const auto pair : list) {
            const auto i = pair.first;
            const auto j = pair.second;

            if (detail::nearest_squared_distance(points[i], points[j], period) < dcut * dcut) {
                list_hash.update(i, j);
            }
        }
    });

    std::cout
        << "N. List:\t"
        << list_time << '\t'
        << list_hash.count << '\t'
        << list_hash.hash << '\n';

    // Brute-force

    pair_hash bruteforce_hash;

    const auto bruteforce_time = timeit([&] {
        for (uint32_t i = 0; i < points.size(); i++) {
            for (uint32_t j = 0; j < i; j++) {
                if (detail::nearest_squared_distance(points[i], points[j], period) < dcut * dcut) {
                    bruteforce_hash.update(i, j);
                }
            }
        }
    });

    std::cout
        << "Bruteforce:\t"
        << bruteforce_time << '\t'
        << bruteforce_hash.count << '\t'
        << bruteforce_hash.hash << '\n';


    assert(searcher_hash.count == bruteforce_hash.count);
    assert(searcher_hash.hash == bruteforce_hash.hash);
    assert(list_hash.count == bruteforce_hash.count);
    assert(list_hash.hash == bruteforce_hash.hash);
}
