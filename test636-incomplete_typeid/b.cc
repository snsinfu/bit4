#include <iostream>
#include <typeinfo>

#include "header.hpp"


struct defined_in_b
{
};

void print_b()
{
    std::cout << "(b) Hash: " << typeid(defined_in_b*).hash_code() << '\n';
}
