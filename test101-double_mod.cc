#include <iostream>
#include <random>

int main()
{
    std::mt19937_64 random_engine;
    std::uniform_int_distribution<unsigned> number_dist{0, 10000};
    unsigned const smaller_modulo = 239;
    unsigned const larger_modulo = 1024;

    for (int i = 0; i < 10000; ++i) {
        unsigned const number = number_dist(random_engine);
        std::cout << number                  % larger_modulo  << '\t'
                  << number % smaller_modulo % larger_modulo  << '\t'
                  << number                  % smaller_modulo << '\t'
                  << number % larger_modulo  % smaller_modulo << '\n';
    }
}
