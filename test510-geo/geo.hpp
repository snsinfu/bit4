// Basic point geometry for C++11 and later.

// Copyright snsinfu 2018.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INCLUDED_GEO_HPP
#define INCLUDED_GEO_HPP

#include <cmath>
#include <cstddef>
#include <istream>
#include <iterator>
#include <ostream>


namespace geo
{
    // index is the integral type used for indexing vectors and points.
    using index = std::size_t;

    namespace detail
    {
        // identity is the identity metafunction. This is used to get around
        // ambiguous template type deduction errors.
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
        template<typename T, index N, typename... Ts>
        struct repeat
        {
            using type = typename repeat<T, N - 1, T, Ts...>::type;
        };

        template<typename T, typename... Ts>
        struct repeat<T, 0, Ts...>
        {
            using type = type_sequence<Ts...>;
        };

        template<typename T, index N>
        using repeat_t = typename repeat<T, N>::type;
    }

    // basic_coords is a static container of N coordinate values. This is the
    // base class of geo::vector and geo::point.
    template<typename T, index N, typename = detail::repeat_t<T, N>>
    class basic_coords;

    template<typename T, index N, typename... Ts>
    class basic_coords<T, N, detail::type_sequence<Ts...>>
    {
    public:
        // scalar_type is the type T of coordinate values.
        using scalar_type = T;

        // iterator is the type of an iterator used to iterate over coordinate
        // values.
        using iterator = scalar_type*;

        // const_iterator is the type of an iterator used to iterate over
        // coordinate values with only read access.
        using const_iterator = scalar_type const*;

        // dimension is the number N of coordinate values.
        static constexpr index dimension = N;

        // Default constructor initializes all coordinate values to zero.
        basic_coords() = default;

        // This constructor takes exactly N coordinate values.
        basic_coords(Ts... coords) noexcept
            : coords_{coords...}
        {
        }

        // begin returns a random access iterator pointing to the first
        // coordinate value.
        iterator begin() noexcept
        {
            return &coords_[0];
        }

        const_iterator begin() const noexcept
        {
            return &coords_[0];
        }

        // end returns a random access iterator pointing to past the last
        // coordinate value.
        iterator end() noexcept
        {
            return &coords_[0] + dimension;
        }

        const_iterator end() const noexcept
        {
            return &coords_[0] + dimension;
        }

        // Indexing operator returns a reference to the i-th coordinate value.
        // Behavior is undefined if the index is out of bounds.
        scalar_type& operator[](index i)
        {
            return coords_[i];
        }

        scalar_type const& operator[](index i) const
        {
            return coords_[i];
        }

    private:
        scalar_type coords_[dimension] {};
    };

    // Equality comparison `u == v` compares the coordinate values for *exact*
    // floating-point equality.
    template<typename T, index N>
    bool operator==(basic_coords<T, N> const& u, basic_coords<T, N> const& v) noexcept
    {
        for (index i = 0; i < N; ++i) {
            if (u[i] != v[i]) {
                return false;
            }
        }
        return true;
    }

    // Inequality comparison `u != v` compares the coordinate values for *exact*
    // floating-point inequality.
    template<typename T, index N>
    bool operator!=(basic_coords<T, N> const& u, basic_coords<T, N> const& v) noexcept
    {
        return !(u == v);
    }

    // Stream-out operator for basic_coords prints the coordinate values
    // delimited by spaces.
    template<typename Char, typename Tr, typename T, index N>
    std::basic_ostream<Char, Tr>& operator<<(std::basic_ostream<Char, Tr>& os,
                                             basic_coords<T, N> const& coords)
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{os}) {
            auto const delim = os.widen(' ');

            for (index i = 0; i < N; ++i) {
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
    template<typename Char, typename Tr, typename T, index N>
    std::basic_istream<Char, Tr>& operator>>(std::basic_istream<Char, Tr>& is,
                                             basic_coords<T, N>& coords)
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{is}) {
            for (index i = 0; i < N; ++i) {
                is >> coords[i];
            }
        }

        return is;
    }

    // vector is a linear vector in the N-dimensional Euclidean space.
    template<typename T, index N>
    class vector : public basic_coords<T, N>
    {
        using basic_coords_ = basic_coords<T, N>;

    public:
        using typename basic_coords_::scalar_type;
        using typename basic_coords_::iterator;
        using typename basic_coords_::const_iterator;
        using basic_coords_::dimension;
        using basic_coords_::basic_coords_;
        using basic_coords_::begin;
        using basic_coords_::end;
        using basic_coords_::operator[];

        // Addition works component-wise.
        vector& operator+=(vector const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtraction works component-wise.
        vector& operator-=(vector const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }

        // Multiplication broadcasts to all components.
        vector& operator*=(scalar_type a) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] *= a;
            }
            return *this;
        }

        // Division broadcasts to all components. Behavior is undefined if a is
        // zero.
        vector& operator/=(scalar_type a)
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] /= a;
            }
            return *this;
        }

        // dot returns the dot product of this vector and v.
        scalar_type dot(vector const& v) const noexcept
        {
            scalar_type sum = 0;
            for (index i = 0; i < dimension; ++i) {
                sum += (*this)[i] * v[i];
            }
            return sum;
        }

        // squared_norm returns the squared Euclidean norm of this vector. This
        // function is faster to compute than norm.
        scalar_type squared_norm() const noexcept
        {
            return dot(*this);
        }

        // norm returns the Euclidean norm of this vector.
        scalar_type norm() const noexcept
        {
            return std::sqrt(squared_norm());
        }

        // cross returns the cross product of this vector and v. Compilation
        // fails if this function is called on a non-three-dimensional vector.
        vector cross(vector const& v) const noexcept
        {
            static_assert(dimension == 3, "cross product is defined only for "
                                          "three-dimensional vectors");
            return vector{
                (*this)[1] * v[2] - (*this)[2] * v[1],
                (*this)[2] * v[0] - (*this)[0] * v[2],
                (*this)[0] * v[1] - (*this)[1] * v[0]
            };
        }

        // normalize returns the unit vector that is parallel to this vector.
        // Behavior is undefined if this vector is zero.
        vector normalize() const
        {
            return *this * (1 / norm());
        }
    };

    // Identity `+v` returns a copy of vec.
    template<typename T, index N>
    vector<T, N> operator+(vector<T, N> const& v) noexcept
    {
        return v;
    }

    // Negation `-v` returns a negated vector.
    template<typename T, index N>
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
    template<typename T, index N>
    vector<T, N> operator+(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} += v;
    }

    // Subtraction `u - v` returns a vector whose coordinates are the
    // element-wise difference of u and v.
    template<typename T, index N>
    vector<T, N> operator-(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{u} -= v;
    }

    // Multiplication by scalar `v * a` returns a vector v scaled by a.
    template<typename T, index N>
    vector<T, N> operator*(vector<T, N> const& v, detail::identity_t<T> a) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Multiplication by scalar `a * v` returns a vector v scaled by a.
    template<typename T, index N>
    vector<T, N> operator*(detail::identity_t<T> a, vector<T, N> const& v) noexcept
    {
        return vector<T, N>{v} *= a;
    }

    // Division by scalar `v / a` returns a vector v scaled by 1/a. Behavior is
    // undefined if a is zero.
    template<typename T, index N>
    vector<T, N> operator/(vector<T, N> const& v, detail::identity_t<T> a)
    {
        return vector<T, N>{v} /= a;
    }

    // dot returns the dot product of a pair of vectors. This is the same as
    // the member function `u.dot(v)`.
    template<typename T, index N>
    T dot(vector<T, N> const& u, vector<T, N> const& v) noexcept
    {
        return u.dot(v);
    }

    // squared_norm returns the squared Euclidean norm of a vector. This is the
    // same as the member function `v.squared_norm()`.
    template<typename T, index N>
    T squared_norm(vector<T, N> const& v) noexcept
    {
        return v.squared_norm();
    }

    // norm returns the Euclidean norm of a vector. This is the same as the
    // member function `v.norm()`.
    template<typename T, index N>
    T norm(vector<T, N> const& v) noexcept
    {
        return v.norm();
    }

    // cross returns the cross product of a pair of three-dimensional vectors.
    // This is the same as the member function `u.cross(v)`.
    template<typename T>
    vector<T, 3> cross(vector<T, 3> const& u, vector<T, 3> const& v) noexcept
    {
        return u.cross(v);
    }

    // normalize returns the unit vector parallel to v. Behavior is undefined
    // if v is a zero vector. This is the same as the member function
    // `v.normalize()`.
    template<typename T, index N>
    vector<T, N> normalize(vector<T, N> const& v)
    {
        return v.normalize();
    }

    // point is a geometric point in the n-dimensional Euclidean space.
    template<typename T, index N>
    class point : public basic_coords<T, N>
    {
        using basic_coords_ = basic_coords<T, N>;

    public:
        using typename basic_coords_::scalar_type;
        using typename basic_coords_::iterator;
        using typename basic_coords_::const_iterator;
        using basic_coords_::dimension;
        using basic_coords_::basic_coords_;
        using basic_coords_::begin;
        using basic_coords_::end;
        using basic_coords_::operator[];
        using vector_type = vector<T, N>;

        // to_vector returns a vector whose components are the coordinate values
        // of this point.
        vector_type to_vector() const noexcept
        {
            return *this - point{};
        }

        // Addition of a vector displaces this point by v.
        point& operator+=(vector_type const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] += v[i];
            }
            return *this;
        }

        // Subtraction of a vector displaces this point by -v.
        point& operator-=(vector_type const& v) noexcept
        {
            for (index i = 0; i < dimension; ++i) {
                (*this)[i] -= v[i];
            }
            return *this;
        }

        // squared_distance returns the squared Euclidean distance to p. This
        // function is faster to compute than distance.
        scalar_type squared_distance(point const& p) const noexcept
        {
            return (*this - p).squared_norm();
        }

        // distance returns the Euclidean distance to p.
        scalar_type distance(point const& p) const noexcept
        {
            return (*this - p).norm();
        }

    };

    // Addition `p + v` returns the point pointed from p by v.
    template<typename T, index N>
    point<T, N> operator+(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} += v;
    }

    // Subtraction `p - v` returns the point that points to p by v.
    template<typename T, index N>
    point<T, N> operator-(point<T, N> const& p, vector<T, N> const& v) noexcept
    {
        return point<T, N>{p} -= v;
    }

    // Subtraction `p - q ` returns the displacement vector originates at q
    // and pointing to p.
    template<typename T, index N>
    vector<T, N> operator-(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        vector<T, N> vec;
        for (index i = 0; i < N; ++i) {
            vec[i] = p[i] - q[i];
        }
        return vec;
    }

    // squared_distance returns the squared Euclidean distance between two
    // points. This function is faster to compute than distance.
    template<typename T, index N>
    T squared_distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return p.squared_distance(q);
    }

    // distance returns the Euclidean distance between two points.
    template<typename T, index N>
    T distance(point<T, N> const& p, point<T, N> const& q) noexcept
    {
        return p.distance(q);
    }

    // interpolate returns the intermediate point between p and q with given
    // division ratio r. Returns p when r = 0 and q when r = 1.
    template<typename T, index N>
    point<T, N> interpolate(point<T, N> const& p, point<T, N> const& q,
                            detail::identity_t<T> r) noexcept
    {
        point<T, N> div;
        for (index i = 0; i < N; ++i) {
            div[i] = (1 - r) * p[i] + r * q[i];
        }
        return div;
    }

    namespace detail
    {
        // element_point is a SFINAE-friendly metafunction to extract the value
        // type of a range assuming it is an instantiation of the point class
        // template.
        template<typename Range,
                 typename = typename std::iterator_traits<typename Range::iterator>::scalar_type>
        struct element_point;

        template<typename Range, typename T, index N>
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
        using scalar_type = typename Point::scalar_type;
        using vector_type = typename Point::vector_type;

        vector_type cen;
        scalar_type n = 0;

        for (Point const& p : points) {
            cen += p.to_vector();
            n++;
        }

        cen *= 1 / n;

        return Point{} + cen;
    }
}

#endif
