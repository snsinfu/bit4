#include <cmath>
#include <cstdio>


double nearest_image_displacement(double x1, double x2, double span)
{
    return std::remainder(x1 - x2, span);
}


int main()
{
    constexpr double period = 1.0;
    constexpr int count = 1000000000;

    double sum = 0;
    double x1 = 0.7;
    double x2 = 0.7;

    for (int i = 0; i < count; i++) {
        sum += nearest_image_displacement(x1, x2, period);
        x1 += 0.01 * x2;
        x2 -= 0.01 * x1;
    }

    std::printf("%g\n", sum / count);
}
