#include "compute_energy.inl"


static
double evaluate_lennard_jones(double dx, double epsilon, double sigma)
{
    double u = sigma / dx;
    double u6 = u * u * u * u * u * u;
    double u12 = u6 * u6;
    return epsilon * (u12 - 2 * u6);
}
