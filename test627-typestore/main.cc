#include <iostream>

// Type-tagged attribute system. Abusing a function as a convenient tool to
// pack (1) tag type, (2) value type and (3) default value.
//
// Downside:
//  - Cannot deduce default value from tag type.

template<typename T>
struct identity
{
    using type = T;
};

struct table
{
    template<typename Tag, typename T>
    void add(T(& column)(Tag*), typename identity<T>::type val)
    {
        std::cout << "Tag: " << typeid(Tag*).name() << '\n';
        std::cout << "Default: " << column(nullptr) << '\n';
        std::cout << "Value: " << val << '\n';
        std::cout << '\n';
    }

    template<typename Tag>
    void column()
    {
        std::cout << "Tag query: " << typeid(Tag*).name() << '\n';
    }
};

// These functions define a tag type, an associated value type and a default
// value.

namespace foo
{
    inline constexpr double my_column(struct my_column_tag*)
    {
        return 1.0;
    }

    inline constexpr int another_column(struct another_column_tag*)
    {
        return -1;
    }
}


int main()
{
    table tab;

    tab.add(foo::my_column, 1.23);
    tab.add(foo::another_column, 42);

    tab.column<foo::my_column_tag>();
}
