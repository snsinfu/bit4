#include <cstdint>
#include <iostream>
#include <random>

template<typename T>
struct hasher
{
    T p, q, m;

    T operator()(T x, T y) const
    {
        return (p * x + q * y) % m;
    }
};

int main()
{
    std::uint32_t const p = 73856093;
    std::uint32_t const q = 19349669;
    std::uint32_t const m = 263;
    hasher<std::uint32_t> hash32{p, q, m};
    hasher<std::uint64_t> hash64{p, q, m};

    std::random_device random_source;
    std::seed_seq seed_seq{random_source(), random_source(), random_source(), random_source()};
    std::mt19937_64 random_engine{seed_seq};
    std::uniform_int_distribution<std::uint32_t> number_dist;

    for (int i = 0; i < 100000; ++i) {
        std::uint32_t const x = number_dist(random_engine);
        std::uint32_t const y = number_dist(random_engine);
        std::cout << hash32(x, y) << '\t' << hash64(x, y) << '\n';
    }
}
