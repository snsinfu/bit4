#include "compute_energy.inl"


static
double evaluate_lennard_jones(double dx, double epsilon, double sigma)
{
    double u = sigma / dx;
    double u2 = u * u;
    double u6 = u2 * u2 * u2;
    return epsilon * (u6 - 2) * u6;
}
