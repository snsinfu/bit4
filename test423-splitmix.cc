// From this paper: https://doi.org/10.1145/2714064.2660195
// Fast Splittable Pseudorandom Number Generators

#include <cstdint>
#include <cstdio>

using std::uint64_t;

// Linear sequence + 64-bit avalanche function. Generates pretty good pseudo-
// random numbers and is very fast.

struct splitmix64
{
    uint64_t x = 0;

    uint64_t next()
    {
        // This constant, called gamma in the paper, needs to be odd for
        // maintaining the 2^64 period of the generated sequence. The authors
        // arbitrarily choosed round_to_odd(2^64 / golden_ratio) in the paper.
        // Any odd number may work and actually, according to the Table 1 in
        // the paper, x += 1 performs just pretty good in some statistical
        // tests. However, deeper statistical consequence is not investigated
        // so... yeah, the authors do not seem to be quite sure what number
        // should be chosen here.

        x += 0x9e3779b97f4a7c15;

        return mix13(x);
    }

    // MurmurHash's avalanche function. This is a bijection (i.e., invertible),
    // so uniformity of the distribution of the numbers in the generated
    // sequence is assured.
    //
    // https://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
    static inline uint64_t mix13(uint64_t z)
    {
        z ^= z >> 30;
        z *= 0xbf58476d1ce4e5b9;
        z ^= z >> 27;
        z *= 0x94d049bb133111eb;
        z ^= z >> 31;

        return z;
    }

    // Not used but this is the inverse! So mix13 is a bijection.
    static inline uint64_t mix13_inverse(uint64_t z)
    {
        z ^= z >> 31;
        z *= 0x319642b2d24d8ec3; // multiplicative inverse of 0x94d049bb133111eb
        z ^= z >> 27;
        z *= 0x96de1b173f119089; // multiplicative inverse of 0xbf58476d1ce4e5b9
        z ^= z >> 30;

        return z;
    }
};

int main()
{
    splitmix64 rand;

    for (long long i = 0; i < 1'000'000; ++i) {
        std::printf("%llu\n", rand.next());
    }
}
