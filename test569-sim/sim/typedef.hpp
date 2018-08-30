// Basic typedefs

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_SIM_TYPEDEF_HPP
#define INCLUDED_SIM_TYPEDEF_HPP

#include <cstddef>
#include <cstdint>

#include <array_view.hpp>
#include <geo.hpp>


namespace sim
{
    // index is the integral type used for indexing array.
    using index = std::size_t;

    // scalar is the floating-point type used for computation.
    using scalar = double;

    // vector is the type used to store a three-dimensional vector.
    using vector = geo::vector<sim::scalar, 3>;

    // point is the type used to store a point in the three-dimensional Euclidean space.
    using point = geo::point<sim::scalar, 3>;

    // step is the integral type used for counting simulation steps.
    using step = std::int64_t;

    // array_view<T> is a non-owning view into an array of T values.
    template<typename T>
    using array_view = cxx::array_view<T>;
}

#endif
