// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Point in the n-dimensional Euclidean space.

#ifndef INCLUDED_GEO_POINT_HPP
#define INCLUDED_GEO_POINT_HPP

#include <iterator>

#include "coords.hpp"
#include "vector.hpp"


namespace geo
{
    // point is a geometric point in the n-dimensional Euclidean space.
    template<typename T, index_t N>
    class point : public basic_coords<T, N>
    {
        using basic_coords = basic_coords<T, N>;

    public:
        using typename basic_coords::value_type;
        using basic_coords::dimension;
        using basic_coords::basic_coords;
        using basic_coords::begin;
        using basic_coords::end;
        using basic_coords::operator[];
        using vector_type = vector<T, N>;

        // vector returns a vector whose components are the coordinate values
        // of this point.
        vector_type vector() const noexcept
        {
            return *this - point{};
        }

        // Adding a vector displaces this point by v.
        point& operator+=(vector_type const& v) noexcept
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtracting a vector displaces this point by -v.
        point& operator-=(vector_type const& v) noexcept
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }
    };

    // Addition `p + v` returns the point pointed from p by v.
    template<typename T, index_t N>
    point<T, N> operator+(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} += v;
    }

    // Subtraction `p - v` returns the point that points to p by v.
    template<typename T, index_t N>
    point<T, N> operator-(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} -= v;
    }

    // Subtraction `p - q ` returns the displacement vector originates at q
    // and pointing to p.
    template<typename T, index_t N>
    vector<T, N> operator-(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        vector<T, N> vec;
        for (index_t i = 0; i < N; ++i) {
            vec[i] = p[i] - q[i];
        }
        return vec;
    }

    // squared_distance returns the squared Euclidean distance between two
    // points. This function is faster to compute than distance.
    template<typename T, index_t N>
    T squared_distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return squared_norm(p - q);
    }

    // distance returns the Euclidean distance between two points.
    template<typename T, index_t N>
    T distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return norm(p - q);
    }

    // interpolate returns the intermediate point between p and q with given
    // division ratio r. Returns p when r = 0 and q when r = 1.
    template<typename T, index_t N>
    point<T, N> interpolate(point<T, N> const& p, point<T, N> const& q,
                            detail::identity_t<T> r) noexcept
    {
        point<T, N> div;
        for (index_t i = 0; i < N; ++i) {
            div[i] = (1 - r) * p[i] + r * q[i];
        }
        return div;
    }

    namespace detail
    {
        // element_point is a metafunction to extract the value type of a range
        // assuming it is an instantiation of the point class template.
        template<typename Range,
                 typename = typename std::iterator_traits<typename Range::iterator>::value_type>
        struct element_point;

        template<typename Range, typename T, index_t N>
        struct element_point<Range, point<T, N>>
        {
            using type = point<T, N>;
        };

        template<typename Range>
        using element_point_t = typename element_point<Range>::type;
    }

    // centroid returns the centroid of points in given range. Behavior is
    // undefined if the range is empty.
    template<typename Range, typename Point = detail::element_point_t<Range>>
    Point centroid(Range const& points)
    {
        using value_type = typename Point::value_type;
        using vector_type = typename Point::vector_type;

        vector_type cen;
        value_type n = 0;

        for (Point const& p : points) {
            cen += p.vector();
            n++;
        }

        cen *= 1 / n;

        return Point{} + cen;
    }
}

#endif
