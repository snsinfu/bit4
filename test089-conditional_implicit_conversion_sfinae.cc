#include <type_traits>

namespace
{
    template<typename T, bool Cond>
    class value_wrapper
    {
      public:
        using value_type = T;

        template<bool Cond_ = Cond, typename std::enable_if<!Cond_, int>::type = 0>
        explicit value_wrapper(value_type value)
            : value_{value}
        {
        }

        template<bool Cond_ = Cond, typename std::enable_if<Cond_, int>::type = 0>
        value_wrapper(value_type value)
            : value_{value}
        {
        }

        template<bool Cond_ = Cond, typename std::enable_if<Cond_, int>::type = 0>
        operator value_type() const
        {
            return value_;
        }

        value_type value() const
        {
            return value_;
        }

      private:
        value_type value_ {};
    };
}

//------------------------------------------------------------------------------

#include <iostream>

int main()
{
    using explicit_value = value_wrapper<int, false>;
    using implicit_value = value_wrapper<int, true>;

    explicit_value v = explicit_value{123};
    implicit_value w = 456;

    int a = v.value();
    int b = w;

    std::cout << a << '\n';
    std::cout << b << '\n';
}
