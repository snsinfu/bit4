// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef GEO_OBJECTS_POINT_HPP
#define GEO_OBJECTS_POINT_HPP

#include <cmath>

#include "../basic_types.hpp"
#include "vector.hpp"

namespace geo
{
    struct point
    {
        geo::scalar x = 0;
        geo::scalar y = 0;
        geo::scalar z = 0;

        geo::vector vector() const
        {
            return {x, y, z};
        }
    };
}

#endif
