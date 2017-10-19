#include <cmath>

namespace units
{
    //----------------------------------------------------------------
    // Traits
    //----------------------------------------------------------------

    /*
     * Traits class for dimension classes.
     */
    template<typename D>
    struct dimension_traits;

    /*
     * Metafunction for computing the dimension of the product of a D1-quantity
     * and a D2-quantity.
     */
    template<typename D1, typename D2>
    struct product_dimension;

    template<typename D1, typename D2>
    using product_dimension_t = typename product_dimension<D1, D2>::type;

    /*
     * Metafunction for computing the dimension of the quotient of a D1-quantity
     * and a D2-quantity.
     */
    template<typename D1, typename D2>
    struct quotient_dimension;

    template<typename D1, typename D2>
    using quotient_dimension_t = typename quotient_dimension<D1, D2>::type;

    /*
     * Metafunction for computing the dimension of a D-quantity raised to the
     * power of N.
     */
    template<typename D, int N>
    struct power_dimension;

    template<typename D, int N>
    using power_dimension_t = typename power_dimension<D, N>::type;

    /*
     * Metafunction for computing the dimension of the N-th root of a D-quantity.
     */
    template<typename D, int N>
    struct root_dimension;

    template<typename D, int N>
    using root_dimension_t = typename root_dimension<D, N>::type;

    //----------------------------------------------------------------
    // The scalar class
    //----------------------------------------------------------------

    namespace detail
    {
        template<typename T, bool AllowConversion>
        class value_holder;

        template<typename T>
        class value_holder<T, true>
        {
          public:
            using value_type = T;

            value_holder() = default;

            value_holder(value_type value) noexcept
                : value_{value}
            {
            }

            operator value_type() const noexcept
            {
                return value_;
            }

          protected:
            value_type value_;
        };

        template<typename T>
        class value_holder<T, false>
        {
          public:
            using value_type = T;

            value_holder() = default;

            explicit value_holder(value_type value) noexcept
                : value_{value}
            {
            }

          protected:
            value_type value_;
        };
    } // namespace detail

    /*
     * Scalar quantity having type T and physical dimension D.
     */
    template<typename T, typename D>
    class scalar : public detail::value_holder<T, dimension_traits<D>::is_dimensionless>
    {
        using value_holder = detail::value_holder<T, dimension_traits<D>::is_dimensionless>;
        using value_holder::value_;

      public:
        using value_holder::value_holder;

        /*
         * The underlying numeric type T.
         */
        using value_type = T;

        /*
         * The dimension type D.
         */
        using dimension = D;

        /*
         * Returns the numerical value without dimension.
         */
        value_type value() const noexcept
        {
            return value_;
        }

        //----------------------------------------------------------------
        // Arithmetic operators
        //----------------------------------------------------------------

        scalar operator+() const noexcept
        {
            return *this;
        }

        scalar operator-() const noexcept
        {
            return scalar{-value_};
        }

        scalar& operator+=(scalar const& rhs) noexcept
        {
            value_ += rhs.value();
            return *this;
        }

        scalar& operator-=(scalar const& rhs) noexcept
        {
            value_ -= rhs.value();
            return *this;
        }

        scalar& operator*=(value_type rhs) noexcept
        {
            value_ *= rhs;
            return *this;
        }

        scalar& operator/=(value_type rhs)
        {
            value_ /= rhs;
            return *this;
        }
    };

    //----------------------------------------------------------------
    // Comparison operators
    //----------------------------------------------------------------

    template<typename T, typename D>
    bool operator==(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return x.value() == y.value();
    }

    template<typename T, typename D>
    bool operator!=(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return !(x == y);
    }

    template<typename T, typename D>
    bool operator<(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return x.value() < y.value();
    }

    template<typename T, typename D>
    bool operator<=(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return x.value() <= y.value();
    }

    template<typename T, typename D>
    bool operator>(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return x.value() > y.value();
    }

    template<typename T, typename D>
    bool operator>=(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return x.value() >= y.value();
    }

    //----------------------------------------------------------------
    // Arithmetic operators
    //----------------------------------------------------------------

    template<typename T, typename D>
    scalar<T, D> operator+(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return scalar<T, D>(x) += y;
    }

    template<typename T, typename D>
    scalar<T, D> operator-(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return scalar<T, D>(x) -= y;
    }

    template<typename T, typename D>
    scalar<T, D> operator*(scalar<T, D> const& x, typename scalar<T, D>::value_type a) noexcept
    {
        return scalar<T, D>(x) *= a;
    }

    template<typename T, typename D>
    scalar<T, D> operator*(typename scalar<T, D>::value_type a, scalar<T, D> const& x) noexcept
    {
        return scalar<T, D>(x) *= a;
    }

    template<typename T, typename D>
    scalar<T, D> operator/(scalar<T, D> const& x, typename scalar<T, D>::value_type a)
    {
        return scalar<T, D>(x) /= a;
    }

    //----------------------------------------------------------------
    // Dimension-converting arithmetic operators
    //----------------------------------------------------------------

    template<typename T, typename DX, typename DY, typename RD = product_dimension_t<DX, DY>>
    scalar<T, RD> operator*(scalar<T, DX> const& x, scalar<T, DY> const& y) noexcept
    {
        return scalar<T, RD>{x.value() * y.value()};
    }

    template<typename T, typename DX, typename DY, typename RD = quotient_dimension_t<DX, DY>>
    scalar<T, RD> operator/(scalar<T, DX> const& x, scalar<T, DY> const& y)
    {
        return scalar<T, RD>{x.value() / y.value()};
    }

    template<typename T, typename D, typename RD = power_dimension_t<D, -1>>
    scalar<T, RD> operator/(typename scalar<T, D>::value_type a, scalar<T, D> const& x)
    {
        return scalar<T, RD>{a} /= x.value();
    }

    //----------------------------------------------------------------
    // Algebraic math operations
    //----------------------------------------------------------------

    /*
     * Computes the x raised to N-th power with correct dimensions.
     */
    template<int N, typename T, typename D, typename RD = power_dimension_t<D, N>>
    scalar<T, RD> pow(scalar<T, D> const& x)
    {
        return scalar<T, RD>{std::pow(x.value(), N)};
    }

    /*
     * Computes the square root of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = root_dimension_t<D, 2>>
    scalar<T, RD> sqrt(scalar<T, D> const& x)
    {
        return scalar<T, RD>{std::sqrt(x.value())};
    }

    /*
     * Computes the cubic root of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = root_dimension_t<D, 3>>
    scalar<T, RD> cbrt(scalar<T, D> const& x) noexcept
    {
        return scalar<T, RD>{std::cbrt(x.value())};
    }

    /*
     * Computes the absolute value with correct dimensions.
     */
    template<typename T, typename D>
    scalar<T, D> abs(scalar<T, D> const& x) noexcept
    {
        return scalar<T, D>{std::fabs(x.value())};
    }

    /*
     * Computes the square root of the sum of the squares of x and y with correct dimensions.
     */
    template<typename T, typename D>
    scalar<T, D> hypot(scalar<T, D> const& x, scalar<T, D> const& y) noexcept
    {
        return scalar<T, D>{std::hypot(x.value(), y.value())};
    }

    //----------------------------------------------------------------
    // The vector class
    //----------------------------------------------------------------

    template<typename T, typename D, unsigned N>
    class vector
    {
      public:
        using value_type = T;
        using scalar_type = scalar<T, D>;
        using iterator = scalar_type*;
        using const_iterator = scalar_type const*;

        vector() = default;

        // Indexing

        scalar_type& operator[](unsigned index)
        {
            return elements_[index];
        }

        scalar_type const& operator[](unsigned index) const
        {
            return elements_[index];
        }

        // Range interface

        iterator begin() noexcept
        {
            return elements_;
        }

        iterator end() noexcept
        {
            return elements_ + N;
        }

        const_iterator begin() const noexcept
        {
            return elements_;
        }

        const_iterator end() const noexcept
        {
            return elements_ + N;
        }

        //----------------------------------------------------------------

        vector operator+() const noexcept
        {
            return *this;
        }

        vector operator-() const noexcept
        {
            vector negated;
            for (unsigned i = 0; i < N; ++i) {
                negated[i] = -elements_[i];
            }
            return negated;
        }

        vector& operator+=(vector const& rhs) noexcept
        {
            for (unsigned i = 0; i < N; ++i) {
                elements_[i] += rhs.elements_[i];
            }
            return *this;
        }

        vector& operator-=(vector const& rhs) noexcept
        {
            for (unsigned i = 0; i < N; ++i) {
                elements_[i] -= rhs.elements_[i];
            }
            return *this;
        }

        vector& operator*=(value_type scale) noexcept
        {
            for (unsigned i = 0; i < N; ++i) {
                elements_[i] *= scale;
            }
            return *this;
        }

        vector& operator/=(value_type scale)
        {
            for (unsigned i = 0; i < N; ++i) {
                elements_[i] /= scale;
            }
            return *this;
        }

      private:
        scalar<T, D> elements_[N] = {};
    };

    //----------------------------------------------------------------
    // The mechanical_dimension class
    //----------------------------------------------------------------

    /*
     * Dimension for mechanical systems.
     */
    template<int Length, int Mass, int Time>
    struct mechanical_dimension
    {
    };

    template<int L, int M, int T>
    struct dimension_traits<mechanical_dimension<L, M, T>>
    {
        static constexpr bool is_dimensionless = (L == 0 && M == 0 && T == 0);
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
            L % N == 0 && M % N == 0 && T % N == 0, "operation results in fractional dimension");
        using type = mechanical_dimension<L / N, M / N, T / N>;
    };

    namespace dimensions
    {
        using number = mechanical_dimension<0, 0, 0>;
        using length = mechanical_dimension<1, 0, 0>;
        using area = mechanical_dimension<2, 0, 0>;
        using volume = mechanical_dimension<3, 0, 0>;
        using mass = mechanical_dimension<0, 1, 0>;
        using time = mechanical_dimension<0, 0, 1>;
        using speed = mechanical_dimension<1, 0, -1>;
        using acceleration = mechanical_dimension<1, 0, -2>;
        using momentum = mechanical_dimension<1, 1, -1>;
        using force = mechanical_dimension<1, 1, -2>;
        using energy = mechanical_dimension<2, 1, -2>;
    } // namespace dimensions

    using number = scalar<double, dimensions::number>;
    using length = scalar<double, dimensions::length>;
    using area = scalar<double, dimensions::area>;
    using volume = scalar<double, dimensions::volume>;
    using mass = scalar<double, dimensions::mass>;
    using time = scalar<double, dimensions::time>;
    using speed = scalar<double, dimensions::speed>;
    using acceleration = scalar<double, dimensions::acceleration>;
    using momentum = scalar<double, dimensions::momentum>;
    using force = scalar<double, dimensions::force>;
    using energy = scalar<double, dimensions::energy>;
} // namespace units

//----------------------------------------------------------------

#include <type_traits>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

//------------------------------------------------------------------------------
// mechanical_dimension
//------------------------------------------------------------------------------

TEST_CASE("mechanical_dimension: different dimensions have different types")
{
    using L = units::mechanical_dimension<1, 0, 0>;
    using M = units::mechanical_dimension<0, 1, 0>;
    using T = units::mechanical_dimension<0, 0, 1>;
    CHECK_FALSE((std::is_same<L, M>::value));
    CHECK_FALSE((std::is_same<L, T>::value));
    CHECK_FALSE((std::is_same<M, T>::value));
}

TEST_CASE("mechanical_dimension: dimension_traits::is_dimensionless")
{
    using A = units::mechanical_dimension<0, 0, 0>;
    CHECK((units::dimension_traits<A>::is_dimensionless));

    using B = units::mechanical_dimension<1, 0, 0>;
    CHECK_FALSE((units::dimension_traits<B>::is_dimensionless));

    using C = units::mechanical_dimension<0, 1, 0>;
    CHECK_FALSE((units::dimension_traits<C>::is_dimensionless));

    using D = units::mechanical_dimension<0, 0, 1>;
    CHECK_FALSE((units::dimension_traits<D>::is_dimensionless));
}

TEST_CASE("mechanical_dimension: product_dimension")
{
    using units::mechanical_dimension;
    using units::product_dimension_t;

    using A = product_dimension_t<mechanical_dimension<1, 5, 4>, mechanical_dimension<6, 3, 2>>;
    CHECK((std::is_same<A, mechanical_dimension<7, 8, 6>>::value));

    using B = product_dimension_t<mechanical_dimension<7, 8, 6>, mechanical_dimension<-1, -5, -4>>;
    CHECK((std::is_same<B, mechanical_dimension<6, 3, 2>>::value));

    using C = product_dimension_t<mechanical_dimension<7, 8, 6>, mechanical_dimension<0, 0, 0>>;
    CHECK((std::is_same<C, mechanical_dimension<7, 8, 6>>::value));

    using D = product_dimension_t<mechanical_dimension<0, 0, 0>, mechanical_dimension<7, 8, 6>>;
    CHECK((std::is_same<D, mechanical_dimension<7, 8, 6>>::value));
}

TEST_CASE("mechanical_dimension: quotient_dimension")
{
    using units::mechanical_dimension;
    using units::quotient_dimension_t;

    using A = quotient_dimension_t<mechanical_dimension<7, 8, 6>, mechanical_dimension<6, 3, 2>>;
    CHECK((std::is_same<A, mechanical_dimension<1, 5, 4>>::value));

    using B = quotient_dimension_t<mechanical_dimension<6, 3, 2>, mechanical_dimension<-1, -5, -4>>;
    CHECK((std::is_same<B, mechanical_dimension<7, 8, 6>>::value));

    using C = quotient_dimension_t<mechanical_dimension<7, 8, 6>, mechanical_dimension<0, 0, 0>>;
    CHECK((std::is_same<C, mechanical_dimension<7, 8, 6>>::value));

    using D = quotient_dimension_t<mechanical_dimension<0, 0, 0>, mechanical_dimension<7, 8, 6>>;
    CHECK((std::is_same<D, mechanical_dimension<-7, -8, -6>>::value));
}

TEST_CASE("mechanical_dimension: power_dimension")
{
    using units::mechanical_dimension;
    using units::power_dimension_t;

    using A = mechanical_dimension<1, 2, 3>;
    CHECK((std::is_same<power_dimension_t<A, 0>, mechanical_dimension<0, 0, 0>>::value));
    CHECK((std::is_same<power_dimension_t<A, 1>, mechanical_dimension<1, 2, 3>>::value));
    CHECK((std::is_same<power_dimension_t<A, 2>, mechanical_dimension<2, 4, 6>>::value));
    CHECK((std::is_same<power_dimension_t<A, -1>, mechanical_dimension<-1, -2, -3>>::value));
    CHECK((std::is_same<power_dimension_t<A, -2>, mechanical_dimension<-2, -4, -6>>::value));
}

TEST_CASE("mechanical_dimension: root_dimension")
{
    using units::mechanical_dimension;
    using units::root_dimension_t;

    using A = mechanical_dimension<6, 18, 12>;
    CHECK((std::is_same<root_dimension_t<A, 1>, mechanical_dimension<6, 18, 12>>::value));
    CHECK((std::is_same<root_dimension_t<A, 2>, mechanical_dimension<3, 9, 6>>::value));
    CHECK((std::is_same<root_dimension_t<A, 3>, mechanical_dimension<2, 6, 4>>::value));
}

TEST_CASE("mechanical_dimension: predefined dimensions")
{
    namespace D = units::dimensions;
    CHECK((std::is_same<units::mechanical_dimension<0, 0, 0>, D::number>::value));
    CHECK((std::is_same<units::mechanical_dimension<1, 0, 0>, D::length>::value));
    CHECK((std::is_same<units::mechanical_dimension<2, 0, 0>, D::area>::value));
    CHECK((std::is_same<units::mechanical_dimension<3, 0, 0>, D::volume>::value));
    CHECK((std::is_same<units::mechanical_dimension<0, 1, 0>, D::mass>::value));
    CHECK((std::is_same<units::mechanical_dimension<0, 0, 1>, D::time>::value));
    CHECK((std::is_same<units::mechanical_dimension<1, 0, -1>, D::speed>::value));
    CHECK((std::is_same<units::mechanical_dimension<1, 0, -2>, D::acceleration>::value));
    CHECK((std::is_same<units::mechanical_dimension<1, 1, -1>, D::momentum>::value));
    CHECK((std::is_same<units::mechanical_dimension<1, 1, -2>, D::force>::value));
    CHECK((std::is_same<units::mechanical_dimension<2, 1, -2>, D::energy>::value));
}

//------------------------------------------------------------------------------
// scalar
//------------------------------------------------------------------------------

TEST_CASE("scalar: is trivial type")
{
    using number = units::scalar<double, units::mechanical_dimension<0, 0, 0>>;
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    CHECK((std::is_trivial<number>::value));
    CHECK((std::is_trivial<length>::value));
}

TEST_CASE("scalar: is default constructible")
{
    using number = units::scalar<double, units::mechanical_dimension<0, 0, 0>>;
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    number a;
    length b;
    a = number{123};
    b = length{456};
}

TEST_CASE("scalar: value_type")
{
    using A = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using B = units::scalar<float, units::mechanical_dimension<1, 0, 0>>;

    CHECK((std::is_same<A::value_type, double>::value));
    CHECK((std::is_same<B::value_type, float>::value));
}

TEST_CASE("scalar: dimension")
{
    using A = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using B = units::scalar<double, units::mechanical_dimension<0, 1, 0>>;

    CHECK((std::is_same<A::dimension, units::mechanical_dimension<1, 0, 0>>::value));
    CHECK((std::is_same<B::dimension, units::mechanical_dimension<0, 1, 0>>::value));
}

TEST_CASE("scalar: value access")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length a{123};
    CHECK(a.value() == 123);
    CHECK(const_cast<length const&>(a).value() == 123);
}

TEST_CASE("scalar: equality comparison")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length a{123};
    length b{456};

    CHECK(a == length{123});
    CHECK(a != length{456});
    CHECK(b == length{456});

    // Swap the sides
    CHECK(length{123} == a);
    CHECK(length{456} != a);
    CHECK(length{456} == b);
}

TEST_CASE("scalar: ordering")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length a{123};
    length b{456};

    CHECK(a < b);
    CHECK(a <= b);
    CHECK(b > a);
    CHECK(b >= a);

    // Boundary case
    CHECK(a <= length{123});
    CHECK(a >= length{123});
}

TEST_CASE("scalar: unary + operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length var{123};
    CHECK(+var == length{123});

    // Returned value is a copy
    +var = length{456};
    CHECK(var == length{123});
}

TEST_CASE("scalar: unary - operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length var{123};
    CHECK(-var == length{-123});
}

TEST_CASE("scalar: += operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Dimension-checked addition
    length var{123};
    var += length{456};
    CHECK(var == length{579});

    // Must return *this
    length& result = (var += length{789});
    CHECK(&result == &var);
}

TEST_CASE("scalar: -= operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Dimension-checked subtraction
    length var{123};
    var -= length{456};
    CHECK(var == length{-333});

    // Must return *this
    length& result = (var -= length{789});
    CHECK(&result == &var);
}

TEST_CASE("scalar: *= operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Scale by dimensionless number
    length var{123};
    var *= 2;
    CHECK(var == length{246});

    // Must return *this
    length& result = (var *= 3);
    CHECK(&result == &var);
}

TEST_CASE("scalar: /= operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Scale by dimensionless number
    length var{246};
    var /= 2;
    CHECK(var == length{123});

    // Must return *this
    length& result = (var /= 3);
    CHECK(&result == &var);
}

TEST_CASE("scalar: binary + operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Dimension-checked addition
    length const a{123};
    length const b{456};
    CHECK(a + b == length{579});
}

TEST_CASE("scalar: binary - operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Dimension-checked subtraction
    length const a{123};
    length const b{456};
    CHECK(a - b == length{-333});
}

TEST_CASE("scalar: binary * operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Scale by dimensionless number
    length const a{123};
    CHECK(a * 2 == length{246});
    CHECK(3 * a == length{369});
}

TEST_CASE("scalar: binary / operator")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    // Scale by dimensionless number
    length const a{246};
    CHECK(a / 2 == length{123});
}

TEST_CASE("scalar: binary * operator with conversion")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using rate = units::scalar<double, units::mechanical_dimension<0, 0, -1>>;
    using speed = units::scalar<double, units::mechanical_dimension<1, 0, -1>>;

    // Product has different dimension
    length const a{3};
    rate const b{5};
    CHECK(a * b == speed{15});
    CHECK(b * a == speed{15});
}

TEST_CASE("scalar: binary / operator with conversion")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using rate = units::scalar<double, units::mechanical_dimension<0, 0, -1>>;
    using time = units::scalar<double, units::mechanical_dimension<0, 0, 1>>;
    using speed = units::scalar<double, units::mechanical_dimension<1, 0, -1>>;

    // Quotient has different dimension
    speed const a{8};
    length const b{4};
    CHECK(a / b == rate{2});
    CHECK(b / a == time{0.5});
}

TEST_CASE("scalar: pow")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using area = units::scalar<double, units::mechanical_dimension<2, 0, 0>>;
    using volume = units::scalar<double, units::mechanical_dimension<3, 0, 0>>;
    using density = units::scalar<double, units::mechanical_dimension<-3, 0, 0>>;

    length const a{2};
    CHECK(units::pow<1>(a) == length{2});
    CHECK(units::pow<2>(a) == area{4});
    CHECK(units::pow<3>(a) == volume{8});

    // 0-th power gives dimensionless number
    CHECK(units::pow<0>(a) == 1);

    // Negative power
    CHECK(units::pow<-3>(a) == density{0.125});
}

TEST_CASE("scalar: sqrt")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using area = units::scalar<double, units::mechanical_dimension<2, 0, 0>>;

    area const a{4};
    CHECK(units::sqrt(a) == length{2});
}

TEST_CASE("scalar: cbrt")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using volume = units::scalar<double, units::mechanical_dimension<3, 0, 0>>;

    volume const a{8};
    CHECK(units::cbrt(a) == length{2});
}

TEST_CASE("scalar: abs")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length const a{4};
    length const b{-5};
    CHECK(units::abs(a) == length{4});
    CHECK(units::abs(b) == length{5});
}

TEST_CASE("scalar: hypot")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;

    length const a{3};
    length const b{4};
    CHECK(units::hypot(a, b) == length{5});
}

TEST_CASE("scalar: dimensional analysis")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using time = units::scalar<double, units::mechanical_dimension<0, 0, 1>>;

    // No conversion
    CHECK_FALSE((std::is_convertible<length, time>::value));
    CHECK_FALSE((std::is_convertible<time, length>::value));

    // No assignment
    CHECK((std::is_assignable<length, length>::value));
    CHECK((std::is_assignable<time, time>::value));
    CHECK_FALSE((std::is_assignable<length, time>::value));
    CHECK_FALSE((std::is_assignable<time, length>::value));
}

TEST_CASE("scalar: implicit conversion of dimensionless quantity")
{
    using length = units::scalar<double, units::mechanical_dimension<1, 0, 0>>;
    using number = units::scalar<double, units::mechanical_dimension<0, 0, 0>>;

    // dimensionless scalar <-> underlying type
    number a = 123;
    double b = a;
    (void) b;

    // No conversion or assignment to/from dimensioned numbers
    CHECK_FALSE((std::is_convertible<number, length>::value));
    CHECK_FALSE((std::is_convertible<length, number>::value));
    CHECK_FALSE((std::is_assignable<number, length>::value));
    CHECK_FALSE((std::is_assignable<length, number>::value));
}

TEST_CASE("scalar: predefined scalars")
{
    // Check dimensions
    CHECK((std::is_same<units::number::dimension, units::dimensions::number>::value));
    CHECK((std::is_same<units::length::dimension, units::dimensions::length>::value));
    CHECK((std::is_same<units::area::dimension, units::dimensions::area>::value));
    CHECK((std::is_same<units::volume::dimension, units::dimensions::volume>::value));
    CHECK((std::is_same<units::mass::dimension, units::dimensions::mass>::value));
    CHECK((std::is_same<units::time::dimension, units::dimensions::time>::value));
    CHECK((std::is_same<units::speed::dimension, units::dimensions::speed>::value));
    CHECK((std::is_same<units::acceleration::dimension, units::dimensions::acceleration>::value));
    CHECK((std::is_same<units::momentum::dimension, units::dimensions::momentum>::value));
    CHECK((std::is_same<units::force::dimension, units::dimensions::force>::value));
    CHECK((std::is_same<units::energy::dimension, units::dimensions::energy>::value));
}

TEST_CASE("scalar: example dimensional analyses")
{
    // Scalars with fundamental dimensions
    units::length length{1};
    units::mass mass{2};
    units::time time{3};

    // Compose other quantities
    units::area area = length * length;
    units::volume volume = area * length;
    units::speed speed = length / time;
    units::acceleration acceleration = speed / time;
    units::momentum momentum = speed * mass;
    units::force force = mass * acceleration;
    units::energy energy = force * length;

    // More cross checks
    force += momentum / time;
    length += units::cbrt(volume);
    momentum += units::sqrt(energy * mass);
}

//------------------------------------------------------------------------------
// vector
//------------------------------------------------------------------------------

TEST_CASE("vector")
{
    using length_vec = units::vector<double, units::dimensions::length, 3>;

    length_vec r;
    r += +r;
    r -= -r;
    r *= 2;
    r /= 2;
}
