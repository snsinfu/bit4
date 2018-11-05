#include <functional>
#include <string>
#include <iostream>


template<typename T>
struct chain
{
    explicit chain(T value)
        : value_{value}
    {
    }

    template<typename Map>
    auto operator->*(Map map) const
    {
        return ::chain(map(value_));
    }

    T resolve() const
    {
        return value_;
    }

private:
    T value_;
};


int main()
{
    auto y = chain(10)
        ->* [](int x) { return x + 1; }
        ->* [](int x) { return x * 2; }
        ->* [](int x) { return std::to_string(x); };

    std::cout << y.resolve() << '\n';
}
