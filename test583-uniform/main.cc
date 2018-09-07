#include <limits>
#include <iostream>
#include <random>


namespace
{
    template<typename T, int N = 64>
    T canonicalize(std::uint64_t bits)
    {
        constexpr int real_bits = std::numeric_limits<T>::digits;
        constexpr int uint_bits = N;
        constexpr int data_bits = (real_bits < uint_bits ? real_bits : uint_bits);

        return T(bits >> (uint_bits - data_bits)) / T(std::int64_t(1) << data_bits);
    }

    template<typename T, typename URNG>
    T generate_canonical(URNG& engine)
    {
        using limits = std::numeric_limits<typename URNG::result_type>;

        static_assert(URNG::min() == 0, "");
        static_assert(URNG::max() == limits::max(), "");

        return canonicalize<T, limits::digits>(engine());
    }
}

int main()
{
    std::mt19937 engine;
    std::cout << generate_canonical<double>(engine) << '\n';
}
