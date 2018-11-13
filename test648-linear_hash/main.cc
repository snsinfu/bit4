#include <iostream>
#include <random>

// Double modulus is not linear!

struct linear_hash
{
    unsigned p = 127;
    unsigned q = 193;
    unsigned m = 2;
    unsigned n = 41;

    unsigned operator()(unsigned x, unsigned y) const
    {
        x += x % m;
        y += y % m;
        return (p * x + q * y) % n;
    }
};


int main()
{
    linear_hash hash;

    std::mt19937_64 random;
    std::uniform_int_distribution<unsigned> uniform{0, 1000};

    for (int i = 0; i < 100; i++) {
        auto x = uniform(random);
        auto y = uniform(random);

        std::cout
            << x
            << '\t'
            << y
            << '\t'
            << hash(x, y)
            << '\t'
            << (hash(x + 1, y) + hash.n - hash(x, y)) % hash.n
            << '\t'
            << (hash(x, y + 1) + hash.n - hash(x, y)) % hash.n
            << '\n';
    }
}
