#include <cmath>
#include <cstdio>


int main()
{
    std::printf("%s\t%s\t%s\n", "x", "rem", "mod");
    for (int i = -10; i <= 10; i++) {
        double const x = i / 10.0;
        double const r = std::remainder(x, 1);
        double const m = std::fmod(x, 1);
        std::printf("%g\t%g\t%g\n", x, r, m);
    }
}
