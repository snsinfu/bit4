#include <iostream>


namespace
{
    void say_hello();
}

int main()
{
    say_hello();
}

namespace // This namespace block is necessary.
{
    void say_hello()
    {
        std::cout << "Hello.\n";
    }
}
