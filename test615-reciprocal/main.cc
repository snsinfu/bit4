// Clang requires -ffinite-math-only to optimize out double division:
//
//     1 / (1 / x) == x .
//
// GCC does not require the option (-funsafe-math-optimizations is enough), so
// this can be a quality-of-implementation issue of clang.
//
// Note. -ffinite-math-only is one of the most dangerous optimization enabled by
// -ffast-math. Avoid that option. So when I write a code for clang I should not
// expect the compiler to optimize double divisions. I should rather carefully
// design functions to minimize double divisions.

#include <cmath>
#include <cstdio>


double compute_gaussian(double r, double sigma)
{
    const double sigma_inv = 1 / sigma;
    const double sigma_inv2 = sigma_inv * sigma_inv;
    return std::exp(-0.5 * sigma_inv2 * r * r);
}


double compute_density(double r, double sigma_inv)
{
    return compute_gaussian(r, 1 / sigma_inv);
}


int main()
{
    std::printf("%g\n", compute_density(1, 0.1));
}
