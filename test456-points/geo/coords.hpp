// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Base class for objects holding n-dimensional coordinate values.

#ifndef INCLUDED_GEO_COORDS_HPP
#define INCLUDED_GEO_COORDS_HPP

#include <cstddef>
#include <functional>
#include <istream>
#include <ostream>

#include "meta.hpp"


namespace geo
{
    // index_t is an integral type of indices.
    using index_t = std::size_t;

    // basic_coords is a static container of N coordinate values.
    template<typename T, index_t N, typename = detail::repeat_t<T, N>>
    class basic_coords;

    template<typename T, index_t N, typename... Ts>
    class basic_coords<T, N, detail::type_sequence<Ts...>>
    {
    public:
        // value_type is the type of a coordinate value.
        using value_type = T;

        // dimension is the number of coordinate values.
        static constexpr index_t dimension = N;

        // Default constructor initializes all coordinate values to zero.
        basic_coords() = default;

        // This constructor takes coordinate values.
        basic_coords(Ts... coords) noexcept
            : coords_{coords...}
        {
        }

        // begin returns a random access iterator pointing to the first
        // coordinate value.
        value_type* begin() noexcept
        {
            return &coords_[0];
        }

        value_type const* begin() const noexcept
        {
            return &coords_[0];
        }

        // end returns a random access iterator pointing to past the last
        // coordinate value.
        value_type* end() noexcept
        {
            return &coords_[0] + dimension;
        }

        value_type const* end() const noexcept
        {
            return &coords_[0] + dimension;
        }

        // Indexing operator returns a reference to the i-th coordinate value.
        // Behavior is undefined for out-of-bounds indices.
        value_type& operator[](index_t i)
        {
            return coords_[i];
        }

        value_type const& operator[](index_t i) const
        {
            return coords_[i];
        }

    private:
        value_type coords_[dimension] {};
    };

    // Equality comparison `lhs == rhs` compares the coordinate values for
    // exact floating-point equality.
    template<typename T, index_t N>
    bool operator==(basic_coords<T, N> const& lhs, basic_coords<T, N> const& rhs) noexcept
    {
        for (index_t i = 0; i < N; ++i) {
            if (lhs[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    // Inequality comparison `lhs != rhs` compares the coordinate values for
    // exact floating-point inequality.
    template<typename T, index_t N>
    bool operator!=(basic_coords<T, N> const& lhs, basic_coords<T, N> const& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    // Stream-out operator for basic_coords prints the coordinate values
    // delimited by spaces.
    template<typename Char, typename Tr, typename T, index_t N>
    std::basic_ostream<Char, Tr>& operator<<(std::basic_ostream<Char, Tr>& os,
                                             basic_coords<T, N> const& coords)
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{os}) {
            auto const delim = os.widen(' ');

            for (index_t i = 0; i < N; ++i) {
                if (i != 0) {
                    os << delim;
                }
                os << coords[i];
            }
        }

        return os;
    }

    // Stream-in operator for basic_coords scans coordinate values delimited by
    // spaces.
    template<typename Char, typename Tr, typename T, index_t N>
    std::basic_istream<Char, Tr>& operator>>(std::basic_istream<Char, Tr>& is,
                                             basic_coords<T, N>& coords)
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{is}) {
            for (index_t i = 0; i < N; ++i) {
                is >> coords[i];
            }
        }

        return is;
    }
}

#endif
