#include <iostream>
#include <random>
#include <utility>

#include <cmath>

double func(double x)
{
    return std::exp(-x * x);
}

std::pair<double, double> monte_carlo_estimate(std::mt19937_64& engine, int n_samples)
{
    std::uniform_real_distribution<double> uniform{0, 1};

    double sum_u = 0;
    double sum_v = 0;

    for (int i = 0; i < n_samples; ++i) {
        double const u = uniform(engine);
        double const v = 1 - u;

        sum_u += func(u);
        sum_v += func(v);
    }

    return std::make_pair(
        sum_u / static_cast<double>(n_samples),
        sum_v / static_cast<double>(n_samples)
    );
}

int main()
{
    std::mt19937_64 engine;

    int const n_estimates = 100;
    int const n_samples = 1000;

    double u1_sum = 0;
    double u2_sum = 0;

    double v1_sum = 0;
    double v2_sum = 0;

    double antithetic1_sum = 0;
    double antithetic2_sum = 0;

    for (int i = 0; i < n_estimates; ++i) {
        auto const[u, v] = monte_carlo_estimate(engine, n_samples);

        u1_sum += u;
        u2_sum += u * u;

        v1_sum += v;
        v2_sum += v * v;

        auto const antithetic = (u + v) / 2;

        antithetic1_sum += antithetic;
        antithetic2_sum += antithetic * antithetic;
    }

    double const u_mean = u1_sum / n_estimates;
    double const v_mean = v1_sum / n_estimates;
    double const antithetic_mean = antithetic1_sum / n_estimates;

    double const u_variance = (u2_sum - u_mean * u1_sum) / n_estimates;
    double const v_variance = (v2_sum - v_mean * v1_sum) / n_estimates;
    double const antithetic_variance = (antithetic2_sum - antithetic_mean * antithetic1_sum) / n_estimates;

    std::cout << u_mean << '\t' << u_variance << '\n';
    std::cout << v_mean << '\t' << v_variance << '\n';
    std::cout << antithetic_mean << '\t' << antithetic_variance << '\n';
}
