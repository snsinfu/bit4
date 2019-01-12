// Compare the performance of modulus operations:
//
// - fmod
// - manual implementation of fmod (fastfmod)
// - integral modulus
//
// fmod is faster than integral modulus when -O2 compiler flag is used, and
// integral modulus is faster when -Os compiler flag is used. fastmod is the
// fastest method in both cases; it is auto-vectorized!


#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <random>
#include <vector>


constexpr std::uint32_t repeat_count = 1000000u;
using clock_type = std::chrono::steady_clock;


double fastmod(double x, double m, double m_inv)
{
    return x - std::floor(x * m_inv) * m;
}


// Consider the periodic system with given unit cell width:
//
//                width
//     ,---------------------------.
// ----|---|---|---|---|---|---|---|---> x
//     `---'
//       ^ dcell
//
// Let us divide each cell into a uniform grid and calculate the subcell index:
//
//      index = int(mod(x, width) / dcell) .
//
// We compare three methods.


// Method 1: fmod
__attribute__((noinline))
void benchmark_fmod(std::vector<double> const& values, double width, double dcell)
{
    auto const start_time = clock_type::now();
    auto const dcell_inv = 1 / dcell;

    std::uint32_t sum = 0;
    for (std::uint32_t repeat = 0; repeat < repeat_count; repeat++) {
        for (std::size_t i = 0; i < values.size(); i++) {
            sum += std::uint32_t(int(std::fmod(values[i], width) * dcell_inv));
        }
    }

    auto const finish_time = clock_type::now();
    auto const elapsed_sec = std::chrono::duration_cast<std::chrono::duration<double>>(finish_time - start_time).count();
    auto const throughput = elapsed_sec / values.size() / repeat_count;

    std::printf("fmod:\n");
    std::printf("  sum = %lu\n", static_cast<unsigned long>(sum));
    std::printf("  t/p = %g ns/op\n", throughput * 1e9);
}


// Method 2: fastmod
__attribute__((noinline))
void benchmark_fastmod(std::vector<double> const& values, double width, double dcell)
{
    auto const start_time = clock_type::now();
    auto const dcell_inv = 1 / dcell;
    auto const width_inv = 1 / width;

    std::uint32_t sum = 0;
    for (std::uint32_t repeat = 0; repeat < repeat_count; repeat++) {
        for (std::size_t i = 0; i < values.size(); i++) {
            sum += std::uint32_t(int(fastmod(values[i], width, width_inv) * dcell_inv));
        }
    }

    auto const finish_time = clock_type::now();
    auto const elapsed_sec = std::chrono::duration_cast<std::chrono::duration<double>>(finish_time - start_time).count();
    auto const throughput = elapsed_sec / values.size() / repeat_count;

    std::printf("fastmod:\n");
    std::printf("  sum = %lu\n", static_cast<unsigned long>(sum));
    std::printf("  t/p = %g ns/op\n", throughput * 1e9);
}


// Method 3: integral modulus
__attribute__((noinline))
void benchmark_modulus(std::vector<double> const& values, double width, double dcell)
{
    auto const start_time = clock_type::now();
    auto const dcell_inv = 1 / dcell;
    auto const mod = std::uint32_t(width / dcell);

    std::uint32_t sum = 0;
    for (std::uint32_t repeat = 0; repeat < repeat_count; repeat++) {
        for (std::size_t i = 0; i < values.size(); i++) {
            sum += std::uint32_t(int(values[i] * dcell_inv)) % mod;
        }
    }

    auto const finish_time = clock_type::now();
    auto const elapsed_sec = std::chrono::duration_cast<std::chrono::duration<double>>(finish_time - start_time).count();
    auto const throughput = elapsed_sec / values.size() / repeat_count;

    std::printf("integral modulus:\n");
    std::printf("  sum = %lu\n", static_cast<unsigned long>(sum));
    std::printf("  t/p = %g ns/op\n", throughput * 1e9);
}


int main()
{
    constexpr double width = 1.0;
    constexpr double dcell = 0.05;

    std::vector<double> values;
    std::mt19937 random;
    std::generate_n(std::back_inserter(values), 1000, [&] {
        std::uniform_real_distribution<double> uniform{0.0, 1.0};
        return uniform(random);
    });

    benchmark_fmod(values, width, dcell);
    benchmark_fastmod(values, width, dcell);
    benchmark_modulus(values, width, dcell);
}
