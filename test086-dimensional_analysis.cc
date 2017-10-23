// DAK: Dimensional Analysis for Mechanics

#include <cmath>

namespace dak
{
    //----------------------------------------------------------------
    // Traits
    //----------------------------------------------------------------

    template<typename D>
    struct dimension_traits;

    template<typename D1, typename D2>
    struct product_dimension;

    template<typename D1, typename D2>
    struct quotient_dimension;

    template<typename D, int N>
    struct power_dimension;

    template<typename D, int N>
    struct root_dimension;

    template<typename D1, typename D2>
    using product_dimension_t = typename product_dimension<D1, D2>::type;

    template<typename D1, typename D2>
    using quotient_dimension_t = typename quotient_dimension<D1, D2>::type;

    template<typename D, int N>
    using power_dimension_t = typename power_dimension<D, N>::type;

    template<typename D, int N>
    using root_dimension_t = typename root_dimension<D, N>::type;

    //----------------------------------------------------------------
    // Scalar quantity with dimensional analysis
    //----------------------------------------------------------------

    namespace detail // for dak::scalar
    {
        // This class mixes in (1) member variable value_ and (2) proper
        // constructors and convertion operator.
        template<typename T, typename D, bool AllowConversion = dimension_traits<D>::is_zero>
        class scalar_mixin;

        template<typename T, typename D>
        class scalar_mixin<T, D, false>
        {
          public:
            scalar_mixin() = default;

            explicit scalar_mixin(T value)
                : value_{value}
            {
            }

          protected:
            T value_{};
        };

        template<typename T, typename D>
        class scalar_mixin<T, D, true>
        {
          public:
            scalar_mixin() = default;

            scalar_mixin(T value) // NOLINT
                : value_{value}
            {
            }

            operator T() const // NOLINT
            {
                return value_;
            }

          protected:
            T value_{};
        };
    } // namespace detail

    /*
     * Scalar quantity with dimensional analysis.
     */
    template<typename T, typename D>
    class scalar : public detail::scalar_mixin<T, D>
    {
        using scalar_mixin = detail::scalar_mixin<T, D>;
        using scalar_mixin::value_;

      public:
        using number_type = T;
        using dimension = D;
        using scalar_mixin::scalar_mixin;

        number_type value() const
        {
            return value_;
        }

        scalar& operator+=(scalar const& rhs)
        {
            value_ += rhs.value_;
            return *this;
        }

        scalar& operator-=(scalar const& rhs)
        {
            value_ -= rhs.value_;
            return *this;
        }

        scalar& operator*=(number_type scale)
        {
            value_ *= scale;
            return *this;
        }

        scalar& operator/=(number_type scale)
        {
            value_ /= scale;
            return *this;
        }
    };

    template<typename T, typename D>
    bool operator==(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return x.value() == y.value();
    }

    template<typename T, typename D>
    bool operator!=(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return !(x == y);
    }

    template<typename T, typename D>
    bool operator<(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return x.value() < y.value();
    }

    template<typename T, typename D>
    bool operator>(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return x.value() > y.value();
    }

    template<typename T, typename D>
    bool operator<=(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return x.value() <= y.value();
    }

    template<typename T, typename D>
    bool operator>=(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return x.value() >= y.value();
    }

    template<typename T, typename D>
    scalar<T, D> operator+(scalar<T, D> const& x)
    {
        return x;
    }

    template<typename T, typename D>
    scalar<T, D> operator-(scalar<T, D> const& x)
    {
        return scalar<T, D>{-x.value()};
    }

    template<typename T, typename D>
    scalar<T, D> operator+(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return scalar<T, D>{x} += y;
    }

    template<typename T, typename D>
    scalar<T, D> operator-(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return scalar<T, D>{x} -= y;
    }

    template<typename T, typename D>
    scalar<T, D> operator*(scalar<T, D> const& x, typename scalar<T, D>::number_type y)
    {
        return scalar<T, D>{x} *= y;
    }

    template<typename T, typename D>
    scalar<T, D> operator*(typename scalar<T, D>::number_type x, scalar<T, D> const& y)
    {
        return scalar<T, D>{y} *= x;
    }

    template<typename T, typename D>
    scalar<T, D> operator/(scalar<T, D> const& x, typename scalar<T, D>::number_type y)
    {
        return scalar<T, D>{x} /= y;
    }

    template<typename T, typename D, typename RD = power_dimension_t<D, -1>>
    scalar<T, RD> operator/(typename scalar<T, D>::number_type x, scalar<T, D> const& y)
    {
        return scalar<T, RD>{x / y.value()};
    }

    template<typename T, typename DX, typename DY, typename RD = product_dimension_t<DX, DY>>
    scalar<T, RD> operator*(scalar<T, DX> const& x, scalar<T, DY> const& y)
    {
        return scalar<T, RD>{x.value() * y.value()};
    }

    template<typename T, typename DX, typename DY, typename RD = quotient_dimension_t<DX, DY>>
    scalar<T, RD> operator/(scalar<T, DX> const& x, scalar<T, DY> const& y)
    {
        return scalar<T, RD>{x.value() / y.value()};
    }

    template<typename T, typename D>
    scalar<T, D> abs(scalar<T, D> const& x)
    {
        return scalar<T, D>{std::fabs(x.value())};
    }

    template<typename T, typename D>
    scalar<T, D> hypot(scalar<T, D> const& x, scalar<T, D> const& y)
    {
        return scalar<T, D>{std::hypot(x.value(), y.value())};
    }

    template<int N, typename T, typename D, typename RD = power_dimension_t<D, N>>
    scalar<T, RD> pow(scalar<T, D> const& x)
    {
        return scalar<T, RD>{std::pow(x.value(), N)};
    }

    template<typename T, typename D, typename RD = root_dimension_t<D, 2>>
    scalar<T, RD> sqrt(scalar<T, D> const& x)
    {
        return scalar<T, RD>{std::sqrt(x.value())};
    }

    template<typename T, typename D, typename RD = root_dimension_t<D, 3>>
    scalar<T, RD> cbrt(scalar<T, D> const& x)
    {
        return scalar<T, RD>{std::cbrt(x.value())};
    }

    //----------------------------------------------------------------
    // Vector quantity with dimensional analysis
    //----------------------------------------------------------------

    namespace detail // for dak::vector
    {
        // Dummy class for passing around type parameter packs. This is used to
        // pack generated parameter types of coords_mixin's constructors.
        template<typename...>
        struct type_sequence
        {
        };

        // Creates type_sequence of T repeated N times.
        template<typename T, unsigned N, typename... Seq>
        struct repeat_type
        {
            using type = typename repeat_type<T, N - 1, T, Seq...>::type;
        };

        template<typename T, typename... Seq>
        struct repeat_type<T, 0, Seq...>
        {
            using type = type_sequence<Seq...>;
        };

        // This class mixes in (1) member variable coords_ and (2) proper
        // constructors.
        template<typename T, typename D, unsigned N, typename = typename repeat_type<T, N>::type>
        class coords_mixin;

        template<typename T, typename D, unsigned N, typename... Ts>
        class coords_mixin<T, D, N, type_sequence<Ts...>>
        {
          public:
            coords_mixin() = default;

            explicit coords_mixin(Ts... coords)
                : coords_{scalar<Ts, D>{coords}...}
            {
            }

            coords_mixin(scalar<Ts, D>... coords) // NOLINT
                : coords_{coords...}
            {
            }

          protected:
            scalar<T, D> coords_[N]{};
        };
    } // namespace detail

    /*
     * Vector with dimensional analysis.
     */
    template<typename T, typename D, unsigned N>
    class vector : private detail::coords_mixin<T, D, N>
    {
        using coords_mixin = detail::coords_mixin<T, D, N>;
        using coords_mixin::coords_;

      public:
        using number_type = T;
        using scalar_type = scalar<T, D>;
        static constexpr unsigned dimension = N;

        using coords_mixin::coords_mixin;

        scalar_type& operator[](unsigned index)
        {
            return coords_[index];
        }

        scalar_type const& operator[](unsigned index) const
        {
            return coords_[index];
        }

        vector& operator+=(vector const& rhs)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] += rhs.coords_[i];
            }
            return *this;
        }

        vector& operator-=(vector const& rhs)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] -= rhs.coords_[i];
            }
            return *this;
        }

        vector& operator*=(number_type scale)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] *= scale;
            }
            return *this;
        }

        vector& operator/=(number_type scale)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] /= scale;
            }
            return *this;
        }
    };

    template<typename T, typename D, unsigned N>
    bool operator==(vector<T, D, N> const& v, vector<T, D, N> const& w)
    {
        for (unsigned i = 0; i < N; ++i) {
            if (v[i] != w[i]) {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename D, unsigned N>
    bool operator!=(vector<T, D, N> const& v, vector<T, D, N> const& w)
    {
        return !(v == w);
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator+(vector<T, D, N> const& v)
    {
        return v;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator-(vector<T, D, N> const& v)
    {
        vector<T, D, N> result;
        for (unsigned i = 0; i < N; ++i) {
            result[i] = -v[i];
        }
        return result;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator+(vector<T, D, N> const& v, vector<T, D, N> const& w)
    {
        return vector<T, D, N>(v) += w;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator-(vector<T, D, N> const& v, vector<T, D, N> const& w)
    {
        return vector<T, D, N>(v) -= w;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator*(vector<T, D, N> const& v, typename vector<T, D, N>::number_type a)
    {
        return vector<T, D, N>(v) *= a;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator*(typename vector<T, D, N>::number_type a, vector<T, D, N> const& v)
    {
        return vector<T, D, N>(v) *= a;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator/(vector<T, D, N> const& v, typename vector<T, D, N>::number_type a)
    {
        return vector<T, D, N>(v) /= a;
    }

    template<typename T, typename D1, typename D2, unsigned N,
        typename RD = product_dimension_t<D1, D2>>
    vector<T, RD, N> operator*(vector<T, D1, N> const& v, scalar<T, D2> const& a)
    {
        vector<T, RD, N> result;
        for (unsigned i = 0; i < N; ++i) {
            result[i] = v[i] * a;
        }
        return result;
    }

    template<typename T, typename D1, typename D2, unsigned N,
        typename RD = product_dimension_t<D1, D2>>
    vector<T, RD, N> operator*(scalar<T, D1> const& a, vector<T, D2, N> const& v)
    {
        vector<T, RD, N> result;
        for (unsigned i = 0; i < N; ++i) {
            result[i] = a * v[i];
        }
        return result;
    }

    template<typename T, typename D1, typename D2, unsigned N,
        typename RD = quotient_dimension_t<D1, D2>>
    vector<T, RD, N> operator/(vector<T, D1, N> const& v, scalar<T, D2> const& a)
    {
        vector<T, RD, N> result;
        for (unsigned i = 0; i < N; ++i) {
            result[i] = v[i] / a;
        }
        return result;
    }

    template<typename T, typename D1, typename D2, unsigned N,
        typename RD = product_dimension_t<D1, D2>>
    scalar<T, RD> dot(vector<T, D1, N> const& v, vector<T, D2, N> const& w)
    {
        scalar<T, RD> result{0};
        for (unsigned i = 0; i < N; ++i) {
            result += v[i] * w[i];
        }
        return result;
    }

    template<typename T, typename D, unsigned N, typename RD = power_dimension_t<D, 2>>
    scalar<T, RD> squared_norm(vector<T, D, N> const& v)
    {
        return dot(v, v);
    }

    template<typename T, typename D, unsigned N>
    scalar<T, D> norm(vector<T, D, N> const& v)
    {
        return sqrt(squared_norm(v));
    }

    template<typename T, typename D1, typename D2, typename RD = product_dimension_t<D1, D2>>
    vector<T, RD, 3> cross(vector<T, D1, 3> const& v, vector<T, D2, 3> const& w)
    {
        auto const x = v[1] * w[2] - v[2] * w[1];
        auto const y = v[2] * w[0] - v[0] * w[2];
        auto const z = v[0] * w[1] - v[1] * w[0];
        return vector<T, RD, 3>{x, y, z};
    }

    //----------------------------------------------------------------
    // Cartesian point with dimensional analysis
    //----------------------------------------------------------------

    /*
     * Cartesian point with dimensional analysis.
     */
    template<typename T, typename D, unsigned N>
    class point : private detail::coords_mixin<T, D, N>
    {
        using coords_mixin = detail::coords_mixin<T, D, N>;
        using coords_mixin::coords_;

      public:
        using number_type = T;
        using scalar_type = scalar<T, D>;
        using vector_type = vector<T, D, N>;
        static constexpr unsigned dimension = N;

        using coords_mixin::coords_mixin;

        scalar_type& operator[](unsigned index)
        {
            return coords_[index];
        }

        scalar_type const& operator[](unsigned index) const
        {
            return coords_[index];
        }

        point& operator+=(vector_type const& rhs)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] += rhs[i];
            }
            return *this;
        }

        point& operator-=(vector_type const& rhs)
        {
            for (unsigned i = 0; i < dimension; ++i) {
                coords_[i] -= rhs[i];
            }
            return *this;
        }
    };

    template<typename T, typename D, unsigned N>
    bool operator==(point<T, D, N> const& v, point<T, D, N> const& w)
    {
        for (unsigned i = 0; i < N; ++i) {
            if (v[i] != w[i]) {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename D, unsigned N>
    bool operator!=(point<T, D, N> const& v, point<T, D, N> const& w)
    {
        return !(v == w);
    }

    template<typename T, typename D, unsigned N>
    point<T, D, N> operator+(point<T, D, N> const& p, vector<T, D, N> const& v)
    {
        return point<T, D, N>{p} += v;
    }

    template<typename T, typename D, unsigned N>
    vector<T, D, N> operator-(point<T, D, N> const& p, point<T, D, N> const& q)
    {
        vector<T, D, N> result;
        for (unsigned i = 0; i < N; ++i) {
            result[i] = p[i] - q[i];
        }
        return result;
    }

    template<typename T, typename D, unsigned N, typename RD = power_dimension_t<D, 2>>
    scalar<T, RD> squared_distance(point<T, D, N> const& p, point<T, D, N> const& q)
    {
        return squared_norm(p - q);
    }

    template<typename T, typename D, unsigned N>
    scalar<T, D> distance(point<T, D, N> const& p, point<T, D, N> const& q)
    {
        return norm(p - q);
    }

    //----------------------------------------------------------------
    // Reference dimension implementation
    //----------------------------------------------------------------

    template<int L, int M, int T>
    struct mechanical_dimension
    {
    };

    template<int L, int M, int T>
    struct dimension_traits<mechanical_dimension<L, M, T>>
    {
        static constexpr bool is_zero = (L == 0 && M == 0 && T == 0);
    };

    template<int L1, int M1, int T1, int L2, int M2, int T2>
    struct product_dimension<mechanical_dimension<L1, M1, T1>, mechanical_dimension<L2, M2, T2>>
    {
        using type = mechanical_dimension<L1 + L2, M1 + M2, T1 + T2>;
    };

    template<int L1, int M1, int T1, int L2, int M2, int T2>
    struct quotient_dimension<mechanical_dimension<L1, M1, T1>, mechanical_dimension<L2, M2, T2>>
    {
        using type = mechanical_dimension<L1 - L2, M1 - M2, T1 - T2>;
    };

    template<int L, int M, int T, int N>
    struct power_dimension<mechanical_dimension<L, M, T>, N>
    {
        using type = mechanical_dimension<L * N, M * N, T * N>;
    };

    template<int L, int M, int T, int N>
    struct root_dimension<mechanical_dimension<L, M, T>, N>
    {
        static_assert(
            L % N == 0 && M % N == 0 && T % N == 0, "fractional dimension is not supported");
        using type = mechanical_dimension<L / N, M / N, T / N>;
    };

    namespace dim
    {
        using length = mechanical_dimension<1, 0, 0>;
        using mass = mechanical_dimension<0, 1, 0>;
        using time = mechanical_dimension<0, 0, 1>;
        using speed = quotient_dimension_t<length, time>;
        using acceleration = quotient_dimension_t<speed, time>;
        using momentum = product_dimension_t<speed, mass>;
        using force = product_dimension_t<acceleration, mass>;
        using energy = product_dimension_t<force, length>;
    } // namespace dim
} // namespace dak

//------------------------------------------------------------------------------
// dak::dim
//------------------------------------------------------------------------------

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("predefined dimensions")
{
    CHECK((std::is_same<dak::dim::length, dak::mechanical_dimension<1, 0, 0>>::value));
    CHECK((std::is_same<dak::dim::mass, dak::mechanical_dimension<0, 1, 0>>::value));
    CHECK((std::is_same<dak::dim::time, dak::mechanical_dimension<0, 0, 1>>::value));
    CHECK((std::is_same<dak::dim::speed, dak::mechanical_dimension<1, 0, -1>>::value));
    CHECK((std::is_same<dak::dim::acceleration, dak::mechanical_dimension<1, 0, -2>>::value));
    CHECK((std::is_same<dak::dim::momentum, dak::mechanical_dimension<1, 1, -1>>::value));
    CHECK((std::is_same<dak::dim::force, dak::mechanical_dimension<1, 1, -2>>::value));
    CHECK((std::is_same<dak::dim::energy, dak::mechanical_dimension<2, 1, -2>>::value));
}

//------------------------------------------------------------------------------
// dak::scalar
//------------------------------------------------------------------------------

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("scalar: is trivially copyable")
{
    using float_length_t = dak::scalar<float, dak::mechanical_dimension<1, 0, 0>>;
    using double_length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_trivially_copyable<float_length_t>::value));
    CHECK((std::is_trivially_copyable<double_length_t>::value));
}

TEST_CASE("scalar: is explicitly constructible with a value")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_constructible<number_t, double>::value));
    CHECK((std::is_constructible<length_t, double>::value));
}

TEST_CASE("scalar: is default constructed to zero")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    length_t x;
    CHECK(x == length_t{0});
}

TEST_CASE("scalar: disallows dimension-altering conversion")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;

    CHECK_FALSE((std::is_convertible<number_t, length_t>::value));
    CHECK_FALSE((std::is_convertible<length_t, number_t>::value));

    CHECK_FALSE((std::is_convertible<length_t, speed_t>::value));
    CHECK_FALSE((std::is_convertible<speed_t, length_t>::value));

    CHECK_FALSE((std::is_convertible<speed_t, number_t>::value));
    CHECK_FALSE((std::is_convertible<number_t, speed_t>::value));
}

TEST_CASE("scalar: is convertible to/from raw number iff dimension is zero")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("static checks")
    {
        CHECK((std::is_convertible<number_t, double>::value));
        CHECK((std::is_convertible<double, number_t>::value));

        CHECK_FALSE((std::is_convertible<length_t, double>::value));
        CHECK_FALSE((std::is_convertible<double, length_t>::value));
    }

    SUBCASE("double -> scalar conversion")
    {
        number_t num = 123;
        CHECK(num == number_t{123});
        num = 456;
        CHECK(num == number_t{456});
    }

    SUBCASE("scalar -> double conversion")
    {
        number_t num = 123;
        double raw = num;
        CHECK(raw == 123);
    }
}

TEST_CASE("scalar: exposes number_type")
{
    using float_length_t = dak::scalar<float, dak::mechanical_dimension<1, 0, 0>>;
    using double_length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<float_length_t::number_type, float>::value));
    CHECK((std::is_same<double_length_t::number_type, double>::value));
}

TEST_CASE("scalar: exposes dimension")
{
    using number_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 0>>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<number_t::dimension, dak::mechanical_dimension<0, 0, 0>>::value));
    CHECK((std::is_same<length_t::dimension, dak::mechanical_dimension<1, 0, 0>>::value));
}

TEST_CASE("scalar: provides raw-number getter")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const len{123};
    double raw = len.value();
    CHECK(raw == 123);
}

TEST_CASE("scalar: provides equality comparison operators")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{123};
    length_t const y{123};
    length_t const z{456};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("scalar: provides order comparison operators")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("comparing strictly different quantities")
    {
        length_t const x{123};
        length_t const y{456};

        CHECK(x < y);
        CHECK_FALSE(y < x);
        CHECK_FALSE(x > y);
        CHECK(y > x);
        CHECK(x <= y);
        CHECK_FALSE(y <= x);
        CHECK_FALSE(x >= y);
        CHECK(y >= x);
    }

    SUBCASE("comparing equal quantities")
    {
        length_t const x{123};
        length_t const y{123};

        CHECK_FALSE(x < y);
        CHECK_FALSE(y < x);
        CHECK_FALSE(x > y);
        CHECK_FALSE(y > x);
        CHECK(x <= y);
        CHECK(y <= x);
        CHECK(x >= y);
        CHECK(y >= x);
    }
}

TEST_CASE("scalar: provides += operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x += length_t{34});
    CHECK(&x == &y);
    CHECK(x == length_t{46});
}

TEST_CASE("scalar: provides -= operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x -= length_t{34});
    CHECK(&x == &y);
    CHECK(x == length_t{-22});
}

TEST_CASE("scalar: provides *= operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x *= 2);
    CHECK(&x == &y);
    CHECK(x == length_t{24});
}

TEST_CASE("scalar: provides /= operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t x{12};
    length_t& y = (x /= 2);
    CHECK(&x == &y);
    CHECK(x == length_t{6});
}

TEST_CASE("scalar: provides unary + operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const& y = +x;
    CHECK(&x != &y);
    CHECK(y == length_t{12});
}

TEST_CASE("scalar: provides unary - operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y = -x;
    CHECK(y == length_t{-12});
}

TEST_CASE("scalar: provides binary + operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y{34};
    length_t const z = x + y;
    CHECK(z == length_t{46});
}

TEST_CASE("scalar: provides binary - operator")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    length_t const y{34};
    length_t const z = x - y;
    CHECK(z == length_t{-22});
}

TEST_CASE("scalar: provides binary * operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x{12};
    CHECK((2 * x) == length_t{24});
    CHECK((x * 3) == length_t{36});
}

TEST_CASE("scalar: provides binary / operator for scaling")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using wavenumber_t = dak::scalar<double, dak::mechanical_dimension<-1, 0, 0>>;

    length_t const x{12};
    CHECK((x / 3) == length_t{4});
    CHECK((36 / x) == wavenumber_t{3});
}

TEST_CASE("scalar: provides binary * operator for dimensional conversion")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;

    length_t const x{12};
    frequency_t const y{3};
    CHECK((x * y) == speed_t{36});
    CHECK((y * x) == speed_t{36});
}

TEST_CASE("scalar: provides binary / operator for dimensional conversion")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;

    speed_t const x{12};
    length_t const y{3};
    CHECK((x / y) == frequency_t{4});
    CHECK((y / x) == duration_t{3.0 / 12});
}

TEST_CASE("scalar: provides abs function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x = dak::abs(length_t{12});
    length_t const y = dak::abs(length_t{-34});
    CHECK(x == length_t{12});
    CHECK(y == length_t{34});
}

TEST_CASE("scalar: provides hypot function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    length_t const x = dak::hypot(length_t{3}, length_t{4});
    CHECK(x == length_t{5});
}

TEST_CASE("scalar: provides pow function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;
    using volume_t = dak::scalar<double, dak::mechanical_dimension<3, 0, 0>>;
    using density_t = dak::scalar<double, dak::mechanical_dimension<-3, 0, 0>>;

    length_t const x{4};
    CHECK(dak::pow<2>(x) == area_t{16});
    CHECK(dak::pow<3>(x) == volume_t{64});
    CHECK(dak::pow<-3>(x) == density_t{1.0 / 64});
}

TEST_CASE("scalar: provides sqrt function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    area_t const x{9};
    CHECK(dak::sqrt(x) == length_t{3});
}

TEST_CASE("scalar: provides cbrt function")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using volume_t = dak::scalar<double, dak::mechanical_dimension<3, 0, 0>>;

    volume_t const x{125};
    CHECK(dak::cbrt(x) == length_t{5});
}

TEST_CASE("scalar: example: mechanical quantities")
{
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    using mass_t = dak::scalar<double, dak::mechanical_dimension<0, 1, 0>>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    using momentum_t = dak::scalar<double, dak::mechanical_dimension<1, 1, -1>>;
    using force_t = dak::scalar<double, dak::mechanical_dimension<1, 1, -2>>;
    using energy_t = dak::scalar<double, dak::mechanical_dimension<2, 1, -2>>;

    length_t const distance{30};
    mass_t const mass{2};
    duration_t const time{5};
    speed_t const speed = distance / time;
    momentum_t const momentum = speed * mass;
    force_t const force = momentum / time;
    energy_t const energy = force * distance;
    double const some_ratio = dak::sqrt(mass * energy) / momentum;
    (void) some_ratio;
}

//------------------------------------------------------------------------------
// dak::vector
//------------------------------------------------------------------------------

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("vector: defined at least for four dimensions")
{
    using vec1 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using vec2 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using vec3 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using vec4 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    CHECK(sizeof(vec1) > 0);
    CHECK(sizeof(vec2) > 0);
    CHECK(sizeof(vec3) > 0);
    CHECK(sizeof(vec4) > 0);
    (void) vec1{1};
    (void) vec2{1, 2};
    (void) vec3{1, 2, 3};
    (void) vec4{1, 2, 3, 4};
}

TEST_CASE("vector: is trivially copyable")
{
    using float_displace_t = dak::vector<float, dak::mechanical_dimension<1, 0, 0>, 3>;
    using double_displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_trivially_copyable<float_displace_t>::value));
    CHECK((std::is_trivially_copyable<double_displace_t>::value));
}

TEST_CASE("vector: is explicitly constructible with coordinate values")
{
    using number_t = dak::vector<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    number_t{1.0, 2.0, 3.0};
    CHECK((std::is_constructible<number_t, double, double, double>::value));
    CHECK((std::is_constructible<displace_t, double, double, double>::value));
}

TEST_CASE("vector: is default constructed to zero")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    displace_t x;
    CHECK(x == displace_t{0, 0, 0});
}

TEST_CASE("vector: disallows dimension-altering conversion")
{
    using number_t = dak::vector<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    CHECK_FALSE((std::is_convertible<number_t, displace_t>::value));
    CHECK_FALSE((std::is_convertible<displace_t, number_t>::value));

    CHECK_FALSE((std::is_convertible<displace_t, velocity_t>::value));
    CHECK_FALSE((std::is_convertible<velocity_t, displace_t>::value));

    CHECK_FALSE((std::is_convertible<velocity_t, number_t>::value));
    CHECK_FALSE((std::is_convertible<number_t, velocity_t>::value));
}

TEST_CASE("vector: exposes number_type")
{
    using float_displace_t = dak::vector<float, dak::mechanical_dimension<1, 0, -1>, 3>;
    using double_displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    CHECK((std::is_same<float_displace_t::number_type, float>::value));
    CHECK((std::is_same<double_displace_t::number_type, double>::value));
}

TEST_CASE("vector: exposes scalar_type")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    CHECK((std::is_same<displace_t::scalar_type, length_t>::value));
}

TEST_CASE("vector: exposes spatial dimension")
{
    using vec1 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using vec2 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using vec3 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using vec4 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    using vec9 = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 9>;
    CHECK((std::integral_constant<unsigned, vec1::dimension>::value) == 1);
    CHECK((std::integral_constant<unsigned, vec2::dimension>::value) == 2);
    CHECK((std::integral_constant<unsigned, vec3::dimension>::value) == 3);
    CHECK((std::integral_constant<unsigned, vec4::dimension>::value) == 4);
    CHECK((std::integral_constant<unsigned, vec9::dimension>::value) == 9);
}

TEST_CASE("vector: provides indexing")
{
    using speed_t = dak::scalar<double, dak::mechanical_dimension<1, 0, -1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    SUBCASE("mutable")
    {
        velocity_t v;
        v[0] = speed_t{1};
        v[1] = speed_t{2};
        v[2] = speed_t{3};
        CHECK(v[0] == speed_t{1});
        CHECK(v[1] == speed_t{2});
        CHECK(v[2] == speed_t{3});
    }

    SUBCASE("const")
    {
        velocity_t const v{1, 2, 3};
        CHECK(v[0] == speed_t{1});
        CHECK(v[1] == speed_t{2});
        CHECK(v[2] == speed_t{3});
    }
}

TEST_CASE("vector: provides equality comparison operators")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{1, 2, 3};
    displace_t const z{4, 5, 6};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("vector: provides += operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x += displace_t{4, 5, 6});
    CHECK(&x == &y);
    CHECK(x == displace_t{5, 7, 9});
}

TEST_CASE("vector: provides -= operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x -= displace_t{6, 5, 4});
    CHECK(&x == &y);
    CHECK(x == displace_t{-5, -3, -1});
}

TEST_CASE("vector: provides *= operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{1, 2, 3};
    displace_t& y = (x *= 2);
    CHECK(&x == &y);
    CHECK(x == displace_t{2, 4, 6});
}

TEST_CASE("vector: provides /= operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t x{2, 4, 6};
    displace_t& y = (x /= 2);
    CHECK(&x == &y);
    CHECK(x == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides unary + operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const& y = +x;
    CHECK(&x != &y);
    CHECK(y == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides unary - operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y = -x;
    CHECK(y == displace_t{-1, -2, -3});
}

TEST_CASE("vector: provides binary + operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK((x + y) == displace_t{5, 7, 9});
}

TEST_CASE("vector: provides binary - operator")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{5, 7, 9};
    displace_t const y{1, 2, 3};
    CHECK((x - y) == displace_t{4, 5, 6});
}

TEST_CASE("vector: provides binary * operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    CHECK((2 * x) == displace_t{2, 4, 6});
    CHECK((x * 3) == displace_t{3, 6, 9});
}

TEST_CASE("vector: provides binary / operator for scaling")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    displace_t const x{3, 6, 9};
    CHECK((x / 3) == displace_t{1, 2, 3});
}

TEST_CASE("vector: provides binary * operator for dimensional conversion")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using frequency_t = dak::scalar<double, dak::mechanical_dimension<0, 0, -1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    displace_t const x{1, 2, 3};
    frequency_t const y{3};
    CHECK((x * y) == velocity_t{3, 6, 9});
    CHECK((y * x) == velocity_t{3, 6, 9});
}

TEST_CASE("vector: provides binary / operator for dimensional conversion")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;

    displace_t const x{3, 6, 9};
    duration_t const y{3};
    CHECK((x / y) == velocity_t{1, 2, 3});
}

TEST_CASE("vector: provides dot function")
{
    using force_t = dak::vector<double, dak::mechanical_dimension<1, 1, -2>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using energy_t = dak::scalar<double, dak::mechanical_dimension<2, 1, -2>>;

    force_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK(dak::dot(x, y) == energy_t{4 + 10 + 18});
}

TEST_CASE("vector: provides squared_norm function")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    displace_t const x{1, 2, 3};
    CHECK(dak::squared_norm(x) == area_t{1 + 4 + 9});
}

TEST_CASE("vector: provides norm function")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    displace_t const x{3, 4};
    CHECK(dak::norm(x) == length_t{5});
}

TEST_CASE("vector: provides cross function for three-dimensional vectors")
{
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_vector_t = dak::vector<double, dak::mechanical_dimension<2, 0, 0>, 3>;

    displace_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK(dak::cross(x, y) == area_vector_t{2 * 6 - 3 * 5, 3 * 4 - 1 * 6, 1 * 5 - 2 * 4});

    // Invariant
    CHECK(dak::cross(x, x) == area_vector_t{0, 0, 0});
    CHECK(dak::cross(y, y) == area_vector_t{0, 0, 0});
}

TEST_CASE("vector: example: mechanical quantities")
{
    using mass_t = dak::scalar<double, dak::mechanical_dimension<0, 1, 0>>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using energy_t = dak::scalar<double, dak::mechanical_dimension<2, 1, -2>>;

    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using velocity_t = dak::vector<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    using momentum_t = dak::vector<double, dak::mechanical_dimension<1, 1, -1>, 3>;
    using force_t = dak::vector<double, dak::mechanical_dimension<1, 1, -2>, 3>;

    displace_t const displacement{10, 20, 30};
    mass_t const mass{2};
    duration_t const time{5};
    velocity_t const velocity = displacement / time;
    momentum_t const momentum = velocity * mass;
    force_t const force = momentum / time;
    energy_t const energy = dak::dot(force, displacement);
    double const some_ratio = dak::sqrt(mass * energy) / dak::norm(momentum);
    (void) some_ratio;
}

//------------------------------------------------------------------------------
// dak::point
//------------------------------------------------------------------------------

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("point: defined at least for four dimensions")
{
    using point1 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using point2 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using point3 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using point4 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    CHECK(sizeof(point1) > 0);
    CHECK(sizeof(point2) > 0);
    CHECK(sizeof(point3) > 0);
    CHECK(sizeof(point4) > 0);
    (void) point1{1};
    (void) point2{1, 2};
    (void) point3{1, 2, 3};
    (void) point4{1, 2, 3, 4};
}

TEST_CASE("point: is trivially copyable")
{
    using float_point_t = dak::point<float, dak::mechanical_dimension<1, 0, 0>, 3>;
    using double_point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_trivially_copyable<float_point_t>::value));
    CHECK((std::is_trivially_copyable<double_point_t>::value));
}

TEST_CASE("vector: is explicitly constructible with coordinate values")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    point_t{1.0, 2.0, 3.0};
    CHECK((std::is_constructible<point_t, double, double, double>::value));
}

TEST_CASE("point: is default constructed to zero")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    point_t x;
    CHECK(x == point_t{0, 0, 0});
}

TEST_CASE("point: disallows dimension-altering conversion")
{
    using tuple_t = dak::point<double, dak::mechanical_dimension<0, 0, 0>, 3>;
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    CHECK_FALSE((std::is_convertible<tuple_t, point_t>::value));
    CHECK_FALSE((std::is_convertible<point_t, tuple_t>::value));
}

TEST_CASE("point: exposes number_type")
{
    using float_point_t = dak::point<float, dak::mechanical_dimension<1, 0, -1>, 3>;
    using double_point_t = dak::point<double, dak::mechanical_dimension<1, 0, -1>, 3>;
    CHECK((std::is_same<float_point_t::number_type, float>::value));
    CHECK((std::is_same<double_point_t::number_type, double>::value));
}

TEST_CASE("point: exposes scalar_type")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;
    CHECK((std::is_same<point_t::scalar_type, length_t>::value));
}

TEST_CASE("point: exposes vector_type")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    CHECK((std::is_same<point_t::vector_type, displace_t>::value));
}

TEST_CASE("point: exposes spatial dimension")
{
    using point1 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 1>;
    using point2 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using point3 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using point4 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 4>;
    using point9 = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 9>;
    CHECK((std::integral_constant<unsigned, point1::dimension>::value) == 1);
    CHECK((std::integral_constant<unsigned, point2::dimension>::value) == 2);
    CHECK((std::integral_constant<unsigned, point3::dimension>::value) == 3);
    CHECK((std::integral_constant<unsigned, point4::dimension>::value) == 4);
    CHECK((std::integral_constant<unsigned, point9::dimension>::value) == 9);
}

TEST_CASE("point: provides indexing")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    SUBCASE("mutable")
    {
        point_t p;
        p[0] = length_t{1};
        p[1] = length_t{2};
        p[2] = length_t{3};
        CHECK(p[0] == length_t{1});
        CHECK(p[1] == length_t{2});
        CHECK(p[2] == length_t{3});
    }

    SUBCASE("const")
    {
        point_t const p{1, 2, 3};
        CHECK(p[0] == length_t{1});
        CHECK(p[1] == length_t{2});
        CHECK(p[2] == length_t{3});
    }
}

TEST_CASE("point: provides equality comparison operators")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{1, 2, 3};
    point_t const y{1, 2, 3};
    point_t const z{4, 5, 6};

    CHECK(x == y);
    CHECK(y == x);

    CHECK(x != z);
    CHECK(z != x);
}

TEST_CASE("point: provides += operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t x{1, 2, 3};
    point_t& y = (x += displace_t{4, 5, 6});
    CHECK(&x == &y);
    CHECK(x == point_t{5, 7, 9});
}

TEST_CASE("point: provides -= operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t x{1, 2, 3};
    point_t& y = (x -= displace_t{6, 5, 4});
    CHECK(&x == &y);
    CHECK(x == point_t{-5, -3, -1});
}

TEST_CASE("point: provides binary + operator for translation")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{1, 2, 3};
    displace_t const y{4, 5, 6};
    CHECK((x + y) == point_t{5, 7, 9});
}

TEST_CASE("point: provides binary - operator for computing displacement")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t const x{5, 7, 9};
    point_t const y{1, 2, 3};
    CHECK((x - y) == displace_t{4, 5, 6});
}

TEST_CASE("point: provides squared_distance function")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using area_t = dak::scalar<double, dak::mechanical_dimension<2, 0, 0>>;

    point_t const p{1, 2, 3};
    point_t const q{6, 5, 4};
    CHECK(dak::squared_distance(p, q) == area_t{5 * 5 + 3 * 3 + 1 * 1});
}

TEST_CASE("point: provides distance function")
{
    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 2>;
    using length_t = dak::scalar<double, dak::mechanical_dimension<1, 0, 0>>;

    point_t const p{1, 2};
    point_t const q{4, 6};
    CHECK(dak::distance(p, q) == length_t{5});
}

TEST_CASE("point: example: two-body simulation")
{
    using mass_t = dak::scalar<double, dak::mechanical_dimension<0, 1, 0>>;
    using duration_t = dak::scalar<double, dak::mechanical_dimension<0, 0, 1>>;
    using grav_constant_t = dak::scalar<double, dak::mechanical_dimension<3, -1, -2>>;

    using displace_t = dak::vector<double, dak::mechanical_dimension<1, 0, 0>, 3>;
    using momentum_t = dak::vector<double, dak::mechanical_dimension<1, 1, -1>, 3>;
    using force_t = dak::vector<double, dak::mechanical_dimension<1, 1, -2>, 3>;

    using point_t = dak::point<double, dak::mechanical_dimension<1, 0, 0>, 3>;

    point_t a_position{0, 0, 0};
    point_t b_position{1, 2, 3};
    momentum_t a_momentum{1, 2, 3};
    momentum_t b_momentum{0, 0, 0};

    grav_constant_t const G{1};
    mass_t const a_mass{1};
    mass_t const b_mass{1};
    duration_t const dt{0.01};

    displace_t const r = a_position - b_position;
    force_t const force = -G * a_mass * b_mass * r / dak::pow<3>(dak::norm(r));
    a_momentum += force * dt;
    b_momentum += -force * dt;
    a_position += a_momentum * dt / a_mass;
    b_position += b_momentum * dt / b_mass;
}
