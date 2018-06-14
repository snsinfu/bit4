// https://marc-b-reynolds.github.io/math/2017/09/18/ModInverse.html

#include <cstdint>
#include <iostream>


std::uint64_t multiplicative_inverse(std::uint64_t a)
{
    std::uint64_t x = a;

    x *= 2 - a*x;
    x *= 2 - a*x;
    x *= 2 - a*x;
    x *= 2 - a*x;
    x *= 2 - a*x;

    return x;
}

void demo(std::uint64_t a)
{
    std::uint64_t inv = multiplicative_inverse(a);

    std::cout << "a        = " << std::hex << a   << '\n'
              << "a^-1     = " << std::hex << inv << '\n'
              << "a * a^-1 = " << a * inv << '\n';
}

int main()
{
    demo(0xbf58476d1ce4e5b9);
    demo(0x94d049bb133111eb);
}
