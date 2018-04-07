// Lambda overload

#include <iostream>

template<typename... Fs>
struct overload : Fs...
{
    using Fs::operator()...;
};

template<typename... Fs>
overload(Fs...) -> overload<Fs...>;

int main()
{
    overload fun = {
        [](int num) { return num * 2; },
        [](int lhs, int rhs) { return lhs + rhs; },
    };
    std::cout << fun(1) << '\n';
    std::cout << fun(1, 2) << '\n';
}
