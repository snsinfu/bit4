#include <filesystem>
#include <iostream>

int main()
{
    std::filesystem::path path{"test.cc"};
    std::cout << absolute(path).string() << '\n';
}
