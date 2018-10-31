#include <iostream>
#include <unordered_map>


int main()
{
    std::unordered_map<void const*, int> map;

    char v1 = 0;
    short v2 = 0;
    char v3 = 0;

    map[&v1] = 1;
    map[&v2] = 2;
    map[&v3] = 3;

    for (auto& pair : map) {
        std::cout << pair.first << '\t' << pair.second << '\n';
    }
}
