#include <cmath>
#include <cstdio>


double nearest_image_displacement(double x1, double x2, double span)
{
    return std::remainder(x1 - x2, span);
}


int main()
{
    constexpr double span = 1.0;

    double x1 = 0.1;
    double x2 = 0.7;

    std::printf("%g\n", nearest_image_displacement(x1, x2, span));
}
