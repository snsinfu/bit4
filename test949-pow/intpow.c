#include "compute_energy.inl"


static
double intpow(double x, int n)
{
    double y = x;
    for (int i = 1; i < n; i++) {
        y *= x;
    }
    return y;
}


static
double evaluate_lennard_jones(double dx, double epsilon, double sigma)
{
    double u = sigma / dx;
    double u6 = intpow(u, 6);
    double u12 = intpow(u, 12);
    return epsilon * (u12 - 2 * u6);
}
