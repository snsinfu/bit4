// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Internal metaprogramming utilities.

#ifndef INCLUDED_GEO_META_HPP
#define INCLUDED_GEO_META_HPP

#include <cstddef>

namespace geo
{
    namespace detail
    {
        // identity is the identity metafunction.
        template<typename T>
        struct identity
        {
            using type = T;
        };

        template<typename T>
        using identity_t = typename identity<T>::type;

        // type_sequence holds a compile-time sequence of types.
        template<typename...>
        struct type_sequence;

        // repeat creates a type_sequence of N copies of T.
        template<typename T, std::size_t N, typename... Ts>
        struct repeat
        {
            using type = typename repeat<T, N - 1, T, Ts...>::type;
        };

        template<typename T, typename... Ts>
        struct repeat<T, 0, Ts...>
        {
            using type = type_sequence<Ts...>;
        };

        template<typename T, std::size_t N>
        using repeat_t = typename repeat<T, N>::type;
    }
}

#endif
