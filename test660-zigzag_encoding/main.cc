#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>


std::uint8_t zigzag_encode(std::int8_t n)
{
    auto const u = std::uint8_t(n);
    auto const v = std::uint8_t(u << 1);
    return (u >> 7) ? std::uint8_t(~v) : v;
}


std::int8_t zigzag_decode(std::uint8_t n)
{
    auto const v = std::int8_t(n >> 1);
    return (n & 1) ? std::int8_t(-v - 1) : v;
}


int main()
{
    constexpr std::int8_t const min = std::numeric_limits<std::int8_t>::min();
    constexpr std::int8_t const max = std::numeric_limits<std::int8_t>::max();

    auto const roundtrip = [](std::int8_t n) {
        auto const z = zigzag_encode(n);
        auto const w = zigzag_decode(z);

        std::cout
            << int(n) << '\t'
            << int(z) << '\t'
            << int(w) << '\t'
            << std::bitset<8>(z) << '\n';
    };

    for (std::int8_t n = min; n < max; n++) {
        roundtrip(n);
    }
    roundtrip(max);
}
