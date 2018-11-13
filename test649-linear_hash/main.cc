#include <cstdint>
#include <iostream>
#include <random>


using std::uint32_t;


struct multilinear_hash
{
    uint32_t mx = 16;
    uint32_t my = 32;

    uint32_t operator()(uint32_t x, uint32_t y) const
    {
        x %= mx;
        y %= my;
        return x + mx * y;
    }
};


int main()
{
    multilinear_hash hash;

    std::mt19937_64 random;
    std::uniform_int_distribution<unsigned> uniform{0, 50};

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
            << (hash(x + 1, y) + hash.mx - hash(x, y)) % hash.mx
            << '\t'
            << (hash(x, y + 1) + hash.my - hash(x, y)) % hash.my / hash.mx
            << '\n';
    }
}
