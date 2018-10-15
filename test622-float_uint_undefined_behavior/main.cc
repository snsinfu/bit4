#include <cstdint>
#include <cmath>
#include <iostream>


void print_as_uint(double val, std::uint32_t mod)
{
    // This conversion is UB if val is out of uint32's range.
    auto const uint = std::uint32_t(val);

    // [clang++] Taking a remainder triggers the undefined behavior, making the
    // result always zero!
    auto const uint_mod = uint % mod;

    std::cout
        << val
        << "\t->\t"
        << uint
        << " / "
        << uint_mod
        << '\n';
}


int main()
{
    std::cout << "Valid\n";
    print_as_uint(12.3, 71);

    std::cout << "\nUB due to negative number\n";
    print_as_uint(-1.0, 71);

    std::cout << "\nUB due to overflow\n";
    print_as_uint(1.0e10, 71);
}
