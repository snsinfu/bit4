#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <utility>


namespace
{
    inline constexpr int log2p1(std::uintmax_t bits)
    {
        return bits ? 1 + log2p1(bits >> 1) : 0;
    }

    template<typename T, typename URNG>
    std::pair<unsigned, T> generate_uint_and_canonical(URNG& engine)
    {
        static_assert(URNG::min() == 0, "");
        static_assert((URNG::max() & (URNG::max() + 1)) == 0, "");

        using uint_type = unsigned;
        using float_type = T;
        using word_type = typename URNG::result_type;

        constexpr int random_bits = log2p1(URNG::max());
        constexpr int uint_bits = 8;
        constexpr int float_bits = std::numeric_limits<float_type>::digits;
        constexpr int remaining_bits = random_bits - uint_bits;
        constexpr int canonical_bits = float_bits < remaining_bits ? float_bits : remaining_bits;

        auto const bits = engine();
        auto const uint = uint_type(bits & ((uint_type(1) << uint_bits) - 1));
        auto const canonical = (bits >> (random_bits - canonical_bits)) / T(word_type(1) << canonical_bits);

        return std::make_pair(uint, canonical);
    }
}

int main()
{
    using float_type = float;
    constexpr int sample_count = 10;

    std::ranlux48 engine;

    std::cout << std::setprecision(std::numeric_limits<float_type>::max_digits10);

    for (int i = 0; i < sample_count; i++) {
        auto const rand = generate_uint_and_canonical<float_type>(engine);
        std::cout << rand.first << '\t' << rand.second << '\n';
    }
}
