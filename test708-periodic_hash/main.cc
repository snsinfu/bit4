#include <cstdint>
#include <iostream>


// Periodic additive hash is effectively impossible...

// Additivity       h(x + 1) = h(x) + h(1)      mod M
// Periodicity      h(x + N) = h(x)             mod M
//
//   h(0) = h(N)
//        = h(N - 1) + h(1)
//        = h(N - 2) + 2 h(1)
//        ...
//        = h(0) + N h(1)         mod M
//
// Therefore,
//
//   N h(1) = 0 mod M.
//
// When M and N are mutually prime the only possibility is h(1) = 0 mod M,
// which implies the trivial hash function h(x) = 0 mod M.


std::uint32_t periodic_additive_hash(std::uint32_t x)
{
    constexpr std::uint32_t mod = 101;
    constexpr std::uint32_t period = 16;
    constexpr std::uint32_t factor = 53;

    return (x % period) * factor % mod;
}


int main()
{
    for (std::uint32_t x = 0; x < 40; x++) {
        std::cout << x << '\t' << periodic_additive_hash(x) << '\n';
    }
}
