// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Linear vector in the n-dimensional Euclidean space.

#ifndef INCLUDED_GEO_VECTOR_HPP
#define INCLUDED_GEO_VECTOR_HPP

#include <cmath>

#include "meta.hpp"
#include "coords.hpp"


namespace geo
{
    // vector is a linear vector in the n-dimensional Euclidean space.
    template<typename T, index_t N>
    class vector : public basic_coords<T, N>
    {
        using basic_coords = basic_coords<T, N>;

    public:
        using typename basic_coords::value_type;
        using basic_coords::dimension;
        using basic_coords::basic_coords;
        using basic_coords::begin;
        using basic_coords::end;
        using basic_coords::operator[];

        // Addition.
        vector& operator+=(vector const& v) noexcept
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtraction.
        vector& operator-=(vector const& v) noexcept
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }

        // Scaling.
        vector& operator*=(value_type a) noexcept
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] *= a;
            }
            return *this;
        }

        // Scaling.
        vector& operator/=(value_type a)
        {
            for (index_t i = 0; i < dimension; ++i) {
                (*this)[i] /= a;
            }
            return *this;
        }
    };

    // Identity `+v` returns a copy of vec.
    template<typename T, index_t N>
    vector<T, N> operator+(vector<T, N> const& v) noexcept
    {
        return v;
    }

    // Negation `-v` returns a negated vector.
    template<typename T, index_t N>
    vector<T, N> operator-(vector<T, N> const& v) noexcept
    {
        vector<T, N> neg{v};
        for (T& coord : neg) {
            coord = -coord;
        }
        return neg;
    }

    // Addition `u + v` returns a vector whose coordinates are the element-wise
    // sum of u and v.
    template<typename T, index_t N>
    vector<T, N> operator+(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} += v;
    }

    // Subtraction `u - v` returns a vector whose coordinates are the element-
    // wise difference of u and v.
    template<typename T, index_t N>
    vector<T, N> operator-(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} -= v;
    }

    // Multiplication by scalar `v * a` returns a vector v scaled by a.
    template<typename T, index_t N>
    vector<T, N> operator*(vector<T, N> const& v, detail::identity_t<T> a) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Multiplication by scalar `a * v` returns a vector v scaled by a.
    template<typename T, index_t N>
    vector<T, N> operator*(detail::identity_t<T> v, vector<T, N> const& a) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Division by scalar `v / a` returns a vector v scaled by 1/a. Behavior is
    // undefined if a is zero.
    template<typename T, index_t N>
    vector<T, N> operator/(vector<T, N> const& v, detail::identity_t<T> a)
    {
        return vector<T, N>{v} /= a;
    }

    // dot returns the dot product of a pair of vectors.
    template<typename T, index_t N>
    T dot(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        T sum = 0;
        for (index_t i = 0; i < N; ++i) {
            sum += u[i] * v[i];
        }
        return sum;
    }

    // squared_norm returns the squared Euclidean norm of a vector.
    template<typename T, index_t N>
    T squared_norm(vector<T, N> const& v) noexcept
    {
        return dot(v, v);
    }

    // norm returns the Euclidean norm of a vector.
    template<typename T, index_t N>
    T norm(vector<T, N> const& v) noexcept
    {
        return std::sqrt(squared_norm(v));
    }

    // cross returns the cross product of a pair of three-dimensional vectors.
    template<typename T>
    vector<T, 3> cross(vector<T, 3> const& u, vector<T, 3> const& v) noexcept
    {
        return vector<T, 3>{
            u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]
        };
    }

    // normalize scales a vector to unit length. Behavior is undefined is v is
    // a zero vector.
    template<typename T, index_t N>
    vector<T, N> normalize(vector<T, N> const& v)
    {
        return v * (1 / norm(v));
    }
}

#endif
