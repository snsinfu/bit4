#include <cmath>
#include <cstdint>
#include <iostream>


struct point
{
    double x;
    double y;
};

using std::uint32_t;


uint32_t hash_point(point pt, uint32_t mod)
{
    constexpr uint32_t px = 3799183523;
    constexpr uint32_t py = 4104169943;

    // Offset coordinate value to avoid negative integer. The signed-to-unsigned
    // conversion here would add 2^32 to the negative integer, which would
    // interfere with the following mod operation.
    constexpr double offset = 1073741823;
    auto const x = uint32_t(int32_t(offset + pt.x));
    auto const y = uint32_t(int32_t(offset + pt.y));

    return (x * px + y * py) % mod;
}


int main()
{
    std::cout << hash_point({ 0.9, 1.0}, 100) << '\n';
    std::cout << hash_point({ 0.2, 1.0}, 100) << '\n';
    std::cout << hash_point({-0.6, 1.0}, 100) << '\n';
}
