#include <cstddef>
#include <iostream>

using index_t = std::size_t;

// type_tuple holds a sequence of types.
template<typename...>
struct type_tuple;

// repeat creates a type_tuple of N instances of T.
template<typename T, index_t N, typename... Ts>
struct repeat
{
    using type = typename repeat<T, N - 1, T, Ts...>::type;
};

template<typename T, typename... Ts>
struct repeat<T, 0, Ts...>
{
    using type = type_tuple<Ts...>;
};

template<typename T, index_t N>
using repeat_t = typename repeat<T, N>::type;

// vector is a tuple of N values supporting linear operations.
template<typename T, index_t N, typename = repeat_t<T, N>>
struct vector;

template<typename T, index_t N, typename... Ts>
struct vector<T, N, type_tuple<Ts...>>
{
    using value_type = T;
    static constexpr index_t dimension = N;

    vector(Ts... values)
        : values_{values...}
    {
    }

    value_type const& operator[](index_t i) const
    {
        return values_[i];
    }

private:
    value_type values_[dimension] {};
};

// operator<< writes coordinate values of vec to os.
template<typename T, index_t N>
std::ostream& operator<<(std::ostream& os, vector<T, N> const& vec)
{
    for (index_t i = 0; i < vec.dimension; ++i) {
        if (i != 0) {
            os << '\t';
        }
        os << vec[i];
    }
    return os;
}

int main()
{
    vector<double, 3> vec = {1, 2, 3};
    std::cout << vec << '\n';
}
