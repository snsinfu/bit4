#include <iostream>

#include "config.hpp"


int main()
{
    config c;
    foreach(c, [](auto name, auto&& member) {
        std::cout << name << '\t' << member << '\n';
    });
}
