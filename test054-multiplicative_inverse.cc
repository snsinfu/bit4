#include <iostream>
#include <tuple>

#include <cassert>
#include <cstddef>
#include <cstdint>


namespace
{
    std::uint64_t inverse(std::uint64_t n)
    {
        // https://crypto.stackexchange.com/a/33261/30140
        std::uint32_t x = static_cast<std::uint32_t>((((n + 2) & 4) << 1) + n);

        x = static_cast<std::uint32_t>((2 - n * x) * x);
        x = static_cast<std::uint32_t>((2 - n * x) * x);
        x = static_cast<std::uint32_t>((2 - n * x) * x);

        return (2 - n * x) * x;
    }
}

int main()
{
    std::size_t const n = 12345;
    std::size_t const r = inverse(n);
    assert(n * r == 1);

    for (std::size_t i = 100000; i < 100010; ++i) {

        std::size_t const k = i / n;
        std::size_t const p = i * r;

        std::cout << (i % n)
                  << '\t'
                  << (p - k) * n
                  << '\n';
    }
}
