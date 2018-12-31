#include <iostream>

#include <md.hpp>


// Defined in sub.cc
void subroutine(md::vec const& v);


int main()
{
    md::vec v1 = {1, 2, 3};
    md::vec v2 = {4, 5, 6};
    md::vec v3 = v1 + 0.1 * (v2 - v1);

    // md::vec::norm() is used here and in subroutine()
    std::cout << v3.norm() << '\n';

    subroutine(v3);
}
