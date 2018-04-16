#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <xtensor/xtensor.hpp>


template<typename E>
void foo(E&& expr)
{
    /*
    // No matching constructor for initialization of 'functor_type'
    E closure{expr};
    */

    // OK
    xt::const_xclosure_t<E> closure{static_cast<xt::const_xclosure_t<E>&>(expr)};

    // Keep closure from being optimized out
    std::printf("%g\n", closure[0]);
}

int main()
{
    std::puts("Initial allocation");
    xt::xtensor<double, 1> x = {1, 2, 3};

    std::puts("No copy");
    foo(x);
    foo(x + x);
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
