#include <cstddef>
#include <cstdio>


namespace my
{
    // Minimal perfect hash system for static types.
    struct type_hash
    {
        // This variable is initialized to a unique value for each type.
        template<typename T>
        static const std::size_t hash_v;

        // Returns the number of registered types. This function may not give
        // the correct number before entering the program main.
        static std::size_t size() noexcept
        {
            return counter();
        }

      private:
        static std::size_t& counter() noexcept
        {
            static std::size_t counter_;
            return counter_;
        }
    };

    template<typename T>
    const std::size_t type_hash::hash_v = type_hash::counter()++;
}


int main()
{
    const std::size_t foo = my::type_hash::hash_v<struct foo_tag>;
    const std::size_t bar = my::type_hash::hash_v<struct bar_tag>;
    const std::size_t baz = my::type_hash::hash_v<struct baz_tag>;

    std::printf("Number of hashed types: %zu\n", my::type_hash::size());
    std::printf("foo = %zu\n", foo);
    std::printf("bar = %zu\n", bar);
    std::printf("baz = %zu\n", baz);
}
