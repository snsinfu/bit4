#include <array>
#include <cassert>

#include <xtensor/xtensor.hpp>


int main()
{
    // Default shape = (1, 1)
    xt::xtensor<double, 2> default_init;
    assert(default_init.shape() == (std::array<std::size_t, 2>{1, 1}));

    // Explicitly set to empty shape
    xt::xtensor<double, 2> explicit_empty({0, 0});
    assert(explicit_empty.shape() == (std::array<std::size_t, 2>{0, 0}));

    // Empty content
    xt::xtensor<double, 2> empty_content = {{}};
    assert(explicit_empty.shape() == (std::array<std::size_t, 2>{0, 0}));
}
