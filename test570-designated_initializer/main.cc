// Flags required to use designated initializers:
//
// clang++  -std=c++14 -Wno-c99-extensions
// clang++  -std=c++2a
// g++      -std=c++2a

#include <iostream>

struct parameters
{
    double alpha = 0;
    double beta = 1;
};

int main()
{
    parameters params = {
        .alpha = 10
    };
    std::cout << params.alpha << '\n';
    std::cout << params.beta << '\n';
}
