// array_view interface

#include <cstddef>
#include <utility>

namespace array_view_detail
{
    // Removes top-level pointer. SFINAE friendly.
    template<typename T>
    struct deref;

    template<typename T>
    struct deref<T*>
    {
        using type = T;
    };

    template<typename T>
    using deref_t = typename deref<T>::type;

    // Returns a pointer to the first element of a contiguous container.
    template<typename Cont>
    auto data(Cont& cont) -> decltype(cont.data())
    {
        return cont.data();
    }

    template<typename T, std::size_t N>
    auto data(T (& array)[N]) -> T*
    {
        return array;
    }

    // Returns the number of elements in a container.
    template<typename Cont>
    auto size(Cont& cont) -> decltype(cont.size())
    {
        return cont.size();
    }

    template<typename T, std::size_t N>
    auto size(T (&)[N]) -> std::size_t
    {
        return N;
    }
}

namespace ext
{
    template<typename T>
    class array_view
    {
      public:
        using pointer = T*;
        using size_type = std::size_t;

        array_view(pointer ptr, size_type size)
            : ptr_{ptr}, size_{size}
        {
        }

      private:
        pointer ptr_ = nullptr;
        size_type size_ = 0;
    };

    template<typename Cont,
        typename P = decltype(array_view_detail::data(std::declval<Cont&>())),
        typename S = decltype(array_view_detail::size(std::declval<Cont&>()))>
    array_view<array_view_detail::deref_t<P>> make_array_view(Cont& cont)
    {
        return {array_view_detail::data(cont), array_view_detail::size(cont)};
    }
}

#include <vector>

int main()
{
    double array[123] = {};
    std::vector<int> vector;

    ext::array_view<double> v1 = ext::make_array_view(array);
    ext::array_view<int> v2 = vector; // should this be allowed?

    (void) v1;
    (void) v2;
}
