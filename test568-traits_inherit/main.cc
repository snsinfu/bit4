#include <iostream>


template<typename T>
struct basic_property_traits
{
    using value_type = T;
    static constexpr value_type init {};
};

template<typename T>
struct property_traits : basic_property_traits<T>
{
};

template<>
struct property_traits<struct mass_property> : basic_property_traits<double>
{
    // Override init
    static constexpr value_type init = 1;
};


int main()
{
    property_traits<mass_property>::value_type mass = property_traits<mass_property>::init;

    std::cout << mass << '\n';
}
