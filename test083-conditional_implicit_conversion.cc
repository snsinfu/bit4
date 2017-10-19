namespace
{
    template<typename T, bool AllowConversion>
    class basic_value_holder;

    template<typename T>
    class basic_value_holder<T, true>
    {
      public:
        using value_type = T;

        basic_value_holder(value_type value)
            : value_{value}
        {
        }

        operator value_type() const
        {
            return value_;
        }

      protected:
        value_type value_;
    };

    template<typename T>
    class basic_value_holder<T, false>
    {
      public:
        using value_type = T;

        explicit basic_value_holder(value_type value)
            : value_{value}
        {
        }

      protected:
        value_type value_;
    };

    template<typename T, bool AllowConversion>
    class value_wrapper : public basic_value_holder<T, AllowConversion>
    {
        using basic_value_holder<T, AllowConversion>::value_;

      public:
        using value_type = T;
        using basic_value_holder<T, AllowConversion>::basic_value_holder;

        value_type value() const
        {
            return value_;
        }
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
