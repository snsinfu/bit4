#include <cmath>
#include <iostream>
#include <random>


double generate_normal_tail(std::mt19937_64& engine, double a)
{
    std::uniform_real_distribution<double> uniform;

    for (;;) {
        double const u1 = uniform(engine);
        double const u2 = uniform(engine);

        double const v = -std::log(u1) / a;
        double const w = -std::log(u2);

        if (2 * w > v * v) {
            return a + v;
        }
    }
}


int main()
{
    std::mt19937_64 engine;

    for (int i = 0; i < 10000; i++) {
        std::cout << generate_normal_tail(engine) << '\n';
    }
}
