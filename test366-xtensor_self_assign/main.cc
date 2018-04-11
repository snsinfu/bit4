#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <xtensor/xmath.hpp>
#include <xtensor/xnoalias.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

int main()
{
    xt::xtensor<double, 2> const data = xt::random::rand<double>({100, 99});

    // Naive div-assignment: Just works. Involves an extra allocation though.
    std::cout << "div-assign\n";
    {
        xt::xtensor<double, 2> var = data;
        var /= xt::view(xt::sum(var, {1}), xt::all(), xt::newaxis());
    }

    // Noalias assertion: Unexpected result, as expected.
    std::cout << "noalias div-assign\n";
    {
        xt::xtensor<double, 2> var = data;
        xt::noalias(var) /= xt::view(xt::sum(var, {1}), xt::all(), xt::newaxis());
    }
}

void* operator new(std::size_t size)
{
    std::cout << "+ new(" << size << ")\n";
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}
