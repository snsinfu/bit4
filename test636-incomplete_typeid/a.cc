#include <iostream>
#include <typeinfo>

#include "header.hpp"


void print_a()
{
    std::cout << "(a) Hash: " << typeid(defined_in_b*).hash_code() << '\n';
}
