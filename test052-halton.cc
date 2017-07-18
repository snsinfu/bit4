#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

double halton(unsigned long i, unsigned long radix)
{
    double result = 0;
    double fraction = 1;
    for (; i != 0; i /= radix)
    {
        fraction /= radix;
        result += fraction * static_cast<double>(i % radix);
    }
    return result;
}

TEST_CASE("halton")
{
    CHECK(halton(0, 2) == 0);
    CHECK(halton(1, 2) == doctest::Approx(1. / 2));
    CHECK(halton(2, 2) == doctest::Approx(1. / 4));
    CHECK(halton(3, 2) == doctest::Approx(3. / 4));
    CHECK(halton(4, 2) == doctest::Approx(1. / 8));
    CHECK(halton(5, 2) == doctest::Approx(5. / 8));
    CHECK(halton(6, 2) == doctest::Approx(3. / 8));
    CHECK(halton(7, 2) == doctest::Approx(7. / 8));
    CHECK(halton(8, 2) == doctest::Approx(1. / 16));
    CHECK(halton(9, 2) == doctest::Approx(9. / 16));
}
