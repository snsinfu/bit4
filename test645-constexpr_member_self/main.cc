#include <iostream>

// Relevant SO question:
// https://stackoverflow.com/questions/29432283/c-static-constexpr-field-with-incomplete-type

namespace foo
{
    struct vector
    {
        double x = 0;
        double y = 0;
        double z = 0;

        // error: constexpr variable cannot have non-literal type 'const foo::vector'
        // note: incomplete type 'const foo::vector' is not a literal type
        // static constexpr vector zero {};

        // error: initialization of incomplete type 'const foo::vector'
        // static constexpr vector const& zero = {};

        static vector const zero;
    };

    // Seems to work but isn't this violate ODR if it is used in a header?
    constexpr vector const vector::zero {};
}


int main()
{
    constexpr foo::vector vec = foo::vector::zero;
    (void) vec;
}
