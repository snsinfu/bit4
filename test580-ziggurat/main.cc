#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <type_traits>
#include <utility>


namespace
{
    template<typename F, typename T, typename P>
    std::pair<T, T> bisect(F func, T min, T max, P stop)
    {
        T fmin = func(min);
        T fmax = func(max);

        while (!stop(min, max)) {
            T const mid = (min + max) / 2;
            T const fmid = func(mid);

            if (fmid * fmin < 0) {
                max = mid;
                fmax = fmid;
            } else {
                min = mid;
                fmin = fmid;
            }
        }

        return std::make_pair(min, max);
    }

    template<typename F, typename T>
    std::pair<T, T> find_bracket(
        F func,
        T seed,
        typename std::common_type<T>::type delta,
        typename std::common_type<T>::type scale
    )
    {
        T min = seed;
        T max = seed;

        do {
            min = (min - delta) / scale;
            max = (max + delta) * scale;
        } while (func(min) * func(max) >= 0);

        return std::make_pair(min, max);
    }
}

namespace
{
    using float_t = float;

    constexpr auto reciprocal_gauss = float_t(0.39894228040143267793994605993438186847585863116493);
    constexpr auto reciprocal_sqrt2 = float_t(0.70710678118654752440084436210484903928483593768847);
    constexpr auto tau = float_t(6.2831853071795864769252867665590057683943387987502);

    inline float_t density(float_t x)
    {
        return reciprocal_gauss * std::exp(-x * x / 2);
    }

    inline float_t inverse_density(float_t p)
    {
        return std::sqrt(-std::log(tau * p * p));
    }

    inline float_t tail_mass(float_t x)
    {
        return std::erfc(reciprocal_sqrt2 * x) / 2;
    }

    template<typename URNG>
    float_t sample_from_tail(float_t a, URNG& engine)
    {
        std::exponential_distribution<float_t> expdist;
        float_t x;
        float_t y;
        do {
            x = expdist(engine) / a;
            y = expdist(engine);
        } while (2 * y < x * x);
        return a + x;
    }
}

int main()
{
    constexpr int table_bits = 8;
    constexpr int strip_count = (1 << table_bits) + 1;

    float_t table[strip_count];

    // Build the table of strip edges.

    auto build_table = [&](float_t edge) {
        auto const strip_area = edge * density(edge) + tail_mass(edge);

        table[0] = strip_area / density(edge);
        table[1] = edge;

        auto const upper_bound = density(0);
        auto current_top = density(edge);

        for (int i = 1; i < strip_count; i++) {
            current_top += strip_area / table[i];
            if (current_top > upper_bound) {
                return false;
            }

            table[i + 1] = inverse_density(current_top);
        }

        return true;
    };

    auto objective = [&](float_t x) {
        return build_table(x) ? table[strip_count - 1] : -1;
    };

    auto stop_condition = [&](float_t min, float_t max) {
        return std::nextafter(min, max) >= max;
    };

    constexpr auto bracket_seed = float_t(1);
    constexpr auto extension_rate = float_t(1.1);

    auto const bracket = find_bracket(objective, bracket_seed, 0, extension_rate);
    auto const roots = bisect(objective, bracket.first, bracket.second, stop_condition);

    build_table(roots.second);

    for (int i = 0; i < strip_count; i++) {
        if (i != 0 && i % 8 == 0) {
            std::cout << '\n';
        }
        std::cout << table[i] << ", ";
    }
    std::cout << '\n';
}
