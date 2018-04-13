// xtensor: xfunction's template constructor shadows copy constructor.

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

#include <xtensor/xtensor.hpp>

template<typename T>
T const& const_ref(T const& obj)
{
    return obj;
}

template<typename E>
void foo(E&& expr)
{
    /*
    E copy{expr}; // No matching constructor for initialization of 'functor_type'
    */
    E copy{const_ref(expr)}; // OK

    (void) copy;
}

int main()
{
    xt::xtensor<double, 1> x = {1, 2, 3};

    // The addition is implemented as a binary function. So we are passing an
    // xt::xfunction here.
    foo(x + x);
}
