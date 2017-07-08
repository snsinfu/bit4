#include <iostream>
#include <type_traits>

namespace
{
    template<typename T>
    using void_t = std::conditional_t<true, void, T>;

    template<typename Func, typename Void = void>
    struct parameter_type;

    template<typename Class, typename T>
    struct parameter_type<void(Class::*)(T) const, void>
    {
        using type = T;
    };

    template<typename Class, typename T>
    struct parameter_type<void(Class::*)(T), void>
    {
        using type = T;
    };

    template<typename Class>
    struct parameter_type<Class, void_t<decltype(&Class::operator())>>
    {
        using type = typename parameter_type<decltype(&Class::operator())>::type;
    };

    template<typename Func>
    using parameter_type_t = typename parameter_type<Func>::type;
}

namespace
{
    template<typename Func>
    void call(Func func)
    {
        parameter_type_t<Func> arg = {};
        func(arg);
    }
}

int main()
{
    struct Data1
    {
        int value = 1;
    };

    call([](Data1 data) {
        std::cout << data.value << '\n';
    });

    struct Data2
    {
        char const* str = "hello";
    };

    call([](Data2 data) {
        std::cout << data.str << '\n';
    });
}
