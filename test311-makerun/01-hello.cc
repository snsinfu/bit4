#include <iostream>
#include <string>


namespace
{
    struct message
    {
        std::string content;
        std::string target;
    };
}


int main()
{
    auto[content, target] = message{"Hello", "world"};
    std::cout << content << ", " << target << '\n';
}
