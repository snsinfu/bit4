// P0828 elastic_ingeger
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0828r0.md

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace
{
    template<typename T, typename U>
    struct assert_same;

    template<typename T>
    struct assert_same<T, T>
    {
    };

    template<int Bits>
    using int_least_t =
        std::conditional_t<Bits <  8, std::int_least8_t,
        std::conditional_t<Bits < 16, std::int_least16_t,
        std::conditional_t<Bits < 32, std::int_least32_t,
        std::conditional_t<Bits < 64, std::int_least64_t, void>>>>;

    inline constexpr int max(int x, int y)
    {
        return x > y ? x : y;
    }

    template<int Bits>
    struct elastic_integer
    {
        using value_type = int_least_t<Bits>;

        elastic_integer(value_type value) noexcept
            : value_{value}
        {
        }

        value_type value() const noexcept
        {
            return value_;
        }

      private:
        value_type value_;
    };

    template<int LhsBits, int RhsBits>
    elastic_integer<LhsBits + RhsBits> operator*(
            elastic_integer<LhsBits> lhs,
            elastic_integer<RhsBits> rhs)
    {
        return lhs.value() * rhs.value();
    }

    template<int LhsBits, int RhsBits>
    elastic_integer<max(LhsBits, RhsBits) + 1> operator+(
            elastic_integer<LhsBits> lhs,
            elastic_integer<RhsBits> rhs)
    {
        return lhs.value() + rhs.value();
    }
}

void test_mult_widen_bits()
{
    elastic_integer<15> x = 30000;
    elastic_integer<10> y = 1000;
    auto xy = x * y;
    assert_same<decltype(xy), elastic_integer<25>>();
    assert(xy.value() == x.value() * y.value());
}

void test_add_widen_bits()
{
    elastic_integer<15> x = 30000;
    elastic_integer<10> y = 1000;
    auto sum = x + y;
    assert_same<decltype(sum), elastic_integer<16>>();
    assert(sum.value() == x.value() + y.value());
}

int main()
{
    test_mult_widen_bits();
    test_add_widen_bits();
}
