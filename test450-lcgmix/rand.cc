#include <cstdint>
#include <cstdio>


int main()
{
    using std::uint64_t;

    uint64_t x = 0;

    for (int i = 0; i < 100000; ++i) {

        // LCG
        x += 0xdcad9890c5cd97b9uLL;

        // mix13
        auto r = x;
        r ^= r >> 30;
        r *= 0xbf58476d1ce4e5b9uLL;
        r ^= r >> 27;
        r *= 0x94d049bb133111ebuLL;
        r ^= r >> 31;

        auto u = 0x1p-64 * r;

        std::printf("%g\n", u);
    }
}


