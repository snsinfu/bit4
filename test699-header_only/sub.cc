#include <iostream>

#include <md.hpp>


void subroutine(md::vec const& v)
{
    std::cout << v.norm() << '\n';
}
