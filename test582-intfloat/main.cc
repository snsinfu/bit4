#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>


// Convert bits to floating point number within [0, 1).

using uint_t = std::uint32_t;
using real_t = float;

constexpr int uint_bits = std::numeric_limits<uint_t>::digits;
constexpr int real_bits = std::numeric_limits<real_t>::digits;
constexpr int real_precision = std::numeric_limits<real_t>::max_digits10;

// Divide by 2^n where n is the bit width of uint_t. This may look good but it does not work: It
// can return 1.0 when bits is large because of rounding. libc++ uses this method and an issue is
// reported. https://bugs.llvm.org/show_bug.cgi?id=18767
real_t by_division(uint_t bits)
{
    return real_t(bits) / (real_t(uint_t(-1)) + 1);
}

// Use ldexp. This also suffers from the rounding problem and does not work.
real_t by_ldexp(uint_t bits)
{
    return std::ldexp(real_t(bits), -uint_bits);
}

// Mask lower bits to force round-to-zero. This works!
real_t by_masked_division(uint_t bits)
{
    return real_t(bits >> (uint_bits - real_bits)) / real_t(uint_t(1) << real_bits);
}

// And ldexp works with the lower-bit masking.
real_t by_masked_ldexp(uint_t bits)
{
    return std::ldexp(real_t(bits >> (uint_bits - real_bits)), -real_bits);
}

void experiment(uint_t bits)
{
    auto const report = [&](char const* name, real_t r) {
        std::cout << std::setprecision(real_precision)
                  << name << ":\n"
                  << "r    = " << r << '\n'
                  << "next = " << std::nextafter(r, r + 1) << "\n\n";
    };

    std::cout << "bits = 0x" << std::hex << bits << "\n\n";

    report("by_division", by_division(bits));
    report("by_ldexp", by_ldexp(bits));
    report("by_masked_division", by_masked_division(bits));
    report("by_masked_ldexp", by_masked_ldexp(bits));
}

int main()
{
    experiment(uint_t(-1));
}
