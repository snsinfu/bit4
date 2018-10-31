#include <iostream>
#include "typemap.hpp"

typemap<int>& get_typemap()
{
    static typemap<int> map;
    return map;
}

void set_from_a(int);
int get_from_b();

int main()
{
    set_from_a(4242);
    std::cout << get_from_b() << '\n';
}
