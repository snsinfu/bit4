// xtensor: Let's reduce allocations!

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <xtensor/xmath.hpp>
#include <xtensor/xnoalias.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xshape.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>


int main()
{
    constexpr double epsilon = 1e-6;

    xt::xtensor<double, 2> var{xt::random::rand<double>({100, 90})};
    xt::xtensor<double, 1> norm{xt::static_shape<std::size_t, 1>{var.shape()[0]}};

    std::puts("start");
    for (int i = 0; i < 100; ++i) {
        xt::noalias(norm) = 1 / (xt::sum(var, {1}) + epsilon);
        xt::noalias(var) *= xt::view(norm, xt::all(), xt::newaxis());
    }
    std::puts("end");
}

void* operator new(std::size_t size)
{
    std::printf("+ new(%zu)\n", size);
    return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}
