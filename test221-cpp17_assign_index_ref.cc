#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

template<typename T>
std::pair<std::size_t, T&> access(std::vector<T>& vec, std::size_t idx)
{
    return {idx, vec[idx]};
}

int main()
{
    std::vector<int> vec = {1, 2, 3};
    auto [idx, ref] = access(vec, 1);
    ref *= 10;
    std::cout << idx << '\t' << ref << '\n';
}
