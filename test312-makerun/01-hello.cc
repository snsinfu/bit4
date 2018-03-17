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


int main(int argc, char** argv)
{
    auto[content, target] = message{"Hello", "c++"};
    std::cout << content << ", " << target << '\n';

    for (int i = 1; i < argc; ++i) {
        std::cout << "Arg: " << argv[i] << '\n';
    }
}
