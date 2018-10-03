// Cache friendly loop can improve performance by > 1000x.
// A: 125 us/call
// B: 0.1 us/call

#include <chrono>
#include <cstddef>
#include <cstdio>


double outer_product_sum_A(double const* values, std::size_t count)
{
    double sum = 0;

    for (std::size_t i = 0; i < count; i++) {
        for (std::size_t j = 0; j < count; j++) {
            sum += values[i] * values[j];
        }
    }

    return sum;
}

double outer_product_sum_B(double const* values, std::size_t count)
{
    constexpr std::size_t mod = 16;
    double const* const end = values + count;
    double const* const mod_end = values + count / mod * mod;

    double sum = 0;

    for (double const* r = values; r != mod_end; r += mod) {
        for (double const* q = values; q != end; q++) {
            for (double const* p = r; p != r + mod; p++) {
                sum += (*p) * (*q);
            }
        }
    }

    for (double const* p = mod_end; p != end; p++) {
        for (double const* q = values; q != end; q++) {
            sum += (*p) * (*q);
        }
    }

    return sum;
}

void benchmark(double(& computation)(double const*, std::size_t))
{
    constexpr int measure_count = 1000;
    constexpr std::size_t value_count = 400;
    double values[value_count] {};

    for (std::size_t i = 0; i < value_count; i++) {
        values[i] = double(i + 1) / 10;
    }

    double dummy_sum = 0;

    auto const t_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < measure_count; i++) {
        dummy_sum += computation(values, value_count);
    }

    auto const t_end = std::chrono::high_resolution_clock::now();
    auto const elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start).count();
    auto const mean_time = elapsed_time / measure_count;
    auto const mean_time_us = mean_time / 1e-6;

    std::printf("result = %g\ttime = %g us/call\n", dummy_sum, mean_time_us);
}

int main()
{
    benchmark(outer_product_sum_A);
    benchmark(outer_product_sum_B);
}
