#include <iostream>

namespace foo
{
    template<typename T>
    struct tag_traits
    {
        using value_type = T;
    };

    template<>
    struct tag_traits<struct bar>
    {
        using value_type = double;
    };
}

int main()
{
    foo::tag_traits<foo::bar>::value_type x = 0;
    std::cout << x << '\n';
}
