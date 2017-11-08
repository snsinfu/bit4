// https://stackoverflow.com/questions/12168348/ways-to-do-modulo-multiplication-with-primitive-types
// https://en.wikipedia.org/wiki/Montgomery_modular_multiplication

#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>

#include <libdivide.h>

using uint_t = std::uint32_t;
using uint_double_t = std::uint64_t;

// Double-width integer
uint_t mulmod0(uint_t a, uint_t b, uint_t m)
{
    uint_double_t const ax = a;
    uint_double_t const bx = b;
    uint_double_t const mx = m;
    return static_cast<uint_t>(ax * bx % mx);
}

// From Keith Randall's answer
uint_t mulmod1(uint_t a, uint_t b, uint_t m)
{
    uint_t res = 0;
    while (a != 0) {
        if (a % 2) {
            res = (res + b) % m;
        }
        a /= 2;
        b = (b * 2) % m;
    }
    return res;
}

// From Craig McQueen's answer
uint_t mulmod2(uint_t a, uint_t b, uint_t m)
{
    uint_t res = 0;

    /* Only needed if b may be >= m */
    if (b >= m) {
        if (m > std::numeric_limits<uint_t>::max() / 2u) {
            b -= m;
        } else {
            b %= m;
        }
    }

    while (a != 0) {
        if (a % 2) {
            /* Add b to res, modulo m, without overflow */
            if (b >= m - res) { /* Equiv to if (res + b >= m), without overflow */
                res -= m;
            }
            res += b;
        }
        a /= 2;

        /* Double b, modulo m */
        uint_t temp_b = b;
        if (b >= m - b) { /* Equiv to if (2 * b >= m), without overflow */
            temp_b -= m;
        }
        b += temp_b;
    }
    return res;
}

// libdivide
uint_t mulmod3(uint_t a, uint_t b, uint_t m)
{
    uint_double_t const ax = a;
    uint_double_t const bx = b;
    uint_double_t const product = ax * bx;
    libdivide::divider<uint_double_t> const mx{m};
    uint_double_t const quotient = product - m * (product / mx);
    return static_cast<uint_t>(quotient);
}

//------------------------------------------------------------------------------

class stop_watch
{
    using clock = std::chrono::high_resolution_clock;

  public:
    stop_watch()
        : start_time_{clock::now()}
    {
    }

    double measure() const
    {
        clock::duration const elapsed_time = clock::now() - start_time_;
        return std::chrono::duration_cast<std::chrono::duration<double>>(elapsed_time).count();
    }

  private:
    clock::time_point start_time_;
};

template<uint_t(& mulmod)(uint_t, uint_t, uint_t)>
void benchmark(char const* name, uint_t p, uint_t m)
{
    std::uint32_t const repeat_count = 1000000;

    stop_watch const swatch;
    uint_t dummy_sum = 1;
    for (std::uint32_t repeat = 0; repeat < repeat_count; ++repeat) {
        dummy_sum += mulmod(p, dummy_sum, m);
    }
    double const elapsed_time_nanosec = 1e9 * swatch.measure();
    double const reciprocal_throughput = elapsed_time_nanosec / repeat_count;

    std::cout << name << '\t'
              << reciprocal_throughput << " ns\t"
              << "sum=" << dummy_sum << '\n';
}

int main()
{
    uint_t const p = 73856093;
    uint_t const m = 1023;

    benchmark<mulmod0>("uint64", p, m);
    benchmark<mulmod1>("kr", p, m);
    benchmark<mulmod2>("cmq", p, m);
    benchmark<mulmod3>("libdiv", p, m);
}

/*
uint64	15.3942 ns	sum=545601090
kr	252.043 ns	sum=545601090
cmq	46.0175 ns	sum=545601090
libdiv	4.85987 ns	sum=545601090
*/
