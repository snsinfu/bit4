#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace bad
{
    template<typename T, unsigned Dimension>
    class vector
    {
      public:
        vector() = default;

        vector(std::initializer_list<T> elements)
            : vector(elements, std::make_index_sequence<Dimension>{})
        {
        }

        template<std::size_t... Indices>
        vector(std::initializer_list<T> elements, std::index_sequence<Indices...>)
            : elements_{*(elements.begin() + Indices)...}
        {
        }

      private:
        T elements_[Dimension] {};
    };

    vector<double, 3> make_some_vector(double x, double y, double z)
    {
        return {x, y, z, 123.456}; // excess element is not caught!
    }
}

namespace better
{
    template<typename T, typename V>
    T implicit_cast(V value)
    {
        return value;
    }

    template<typename T, std::size_t N, typename... Types>
    struct is_vector_initializable;

    template<typename T, std::size_t N, typename U, typename... Rest>
    struct is_vector_initializable<T, N, U, Rest...>
    {
        static constexpr bool value =
            std::is_convertible<U, T>::value && is_vector_initializable<T, N - 1, Rest...>::value;
    };

    template<typename T, typename U, typename... Rest>
    struct is_vector_initializable<T, 0, U, Rest...>
    {
        static constexpr bool value = false;
    };

    template<typename T, std::size_t N>
    struct is_vector_initializable<T, N>
    {
        static constexpr bool value = false;
    };

    template<typename T>
    struct is_vector_initializable<T, 0>
    {
        static constexpr bool value = true;
    };

    template<typename T, unsigned Dimension>
    class vector
    {
      public:
        vector() = default;

        template<typename... Init,
                 std::enable_if_t<is_vector_initializable<T, Dimension, Init...>::value, int> = 0>
        vector(Init... elements)
            : elements_{implicit_cast<T>(elements)...}
        {
        }

      private:
        T elements_[Dimension] {};
    };

    vector<double, 3> make_some_vector(double x, double y, double z)
    {
        return {x, y, z};
    }
}

int main()
{
    bad::make_some_vector(1, 2, 3);
    better::make_some_vector(1, 2, 3);
}
