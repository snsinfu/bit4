#include <math.h>

#include "compute_energy.inl"


static
double evaluate_lennard_jones(double dx, double epsilon, double sigma)
{
    double u = sigma / dx;
    double u6 = pow(u, 6);
    double u12 = pow(u, 12);
    return epsilon * (u12 - 2 * u6);
}
