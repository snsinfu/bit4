#include <cmath>
#include <cstdio>
#include <random>


double gaussian_3d_density(double r, double var)
{
    auto const inv_var = 1 / var;
    auto const norm = std::sqrt(inv_var) * inv_var;
    return norm * std::exp(-0.5 * inv_var * r * r);
}

double approx_invsqrt(double x)
{
    // Newton's method with initial value 1
    auto const y = (3 - x) * 0.5;
    return y * (3 - x * y * y) * 0.5;
}

double approx_invsqrt_2(double x)
{
    // Taylor expansion around x = 1
    return 1.875 + x * (0.375 * x - 1.25);
}

double approx_gaussian_3d_density(double r, double var)
{
    auto const inv_sigma = approx_invsqrt(var);
    auto const inv_var = inv_sigma * inv_sigma;
    auto const norm = inv_var * inv_sigma;

    auto const u = 1 - 0.125 * inv_var * r * r;
    auto const v = (u > 0 ? u : 0);

    return norm * (v * v) * (v * v);
}

int main()
{
    std::mt19937_64 engine;
    std::uniform_real_distribution<double> r_distr{0.0, 4.0};
    std::uniform_real_distribution<double> var_distr{0.7, 1.3};

    for (int i = 0; i < 100; i++) {
        auto const r = r_distr(engine);
        auto const var = var_distr(engine);

        auto const f_true = gaussian_3d_density(r, var);
        auto const f_approx = approx_gaussian_3d_density(r, var);
        auto const error = f_approx - f_true;

        std::printf("%.3f\t%.3f\t%+.3f\n", r, var, error);
    }
}
