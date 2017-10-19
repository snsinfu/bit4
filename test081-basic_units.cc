#include <cmath>
#include <type_traits>

namespace units
{
    //----------------------------------------------------------------
    // Traits
    //----------------------------------------------------------------

    /*
     * Traits class for dimension classes.
     *
     * Required static constexpr data member:
     * - bool is_dimensionless
     *
     * Required member alias templates:
     * - power<int N>
     * - root<int N>
     */
    template<typename D>
    struct dimension_traits;

    /*
     * Traits class for arithmetics on quantities having dimensions D1 and D2.
     *
     * Required member types:
     * - product_dimension
     * - quotient_dimension
     */
    template<typename D1, typename D2>
    struct arithmetic_traits;

    //----------------------------------------------------------------
    // The quantity class
    //----------------------------------------------------------------

    /*
     * Scalar quantity having type T and physical dimension D.
     */
    template<typename T, typename D>
    class quantity
    {
      public:
        /*
         * The arithmetic type.
         */
        using value_type = T;

        /*
         * The dimension type.
         */
        using dimension = D;

        /*
         * Default constructor default-initializes the value.
         */
        quantity() = default;

        /*
         * Initializes the value as specified.
         */
        explicit quantity(value_type value) noexcept
            : value_{value}
        {
        }

        /*
         * Returns the stored value.
         */
        value_type value() const noexcept
        {
            return value_;
        }

        //----------------------------------------------------------------
        // Arithmetic operators
        //----------------------------------------------------------------

        quantity operator+() const noexcept
        {
            return *this;
        }

        quantity operator-() const noexcept
        {
            return quantity{-value_};
        }

        quantity& operator+=(quantity const& rhs) noexcept
        {
            value_ += rhs.value_;
            return *this;
        }

        quantity& operator-=(quantity const& rhs) noexcept
        {
            value_ -= rhs.value_;
            return *this;
        }

        quantity& operator*=(value_type rhs) noexcept
        {
            value_ *= rhs;
            return *this;
        }

        quantity& operator/=(value_type rhs)
        {
            value_ /= rhs;
            return *this;
        }

      private:
        value_type value_;
    };

    /*
     * Aliases to `quantity<T, D>` if D is not dimension-less or to `T` if D is
     * dimension-less.
     */
    template<typename T, typename D>
    using quantity_or_scalar = typename std::conditional<
        dimension_traits<D>::is_dimensionless, T, quantity<T, D>
    >::type;

    //----------------------------------------------------------------
    // Comparison operators
    //----------------------------------------------------------------

    template<typename T, typename D>
    bool operator==(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return x.value() == y.value();
    }

    template<typename T, typename D>
    bool operator!=(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return !(x == y);
    }

    template<typename T, typename D>
    bool operator<(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return x.value() < y.value();
    }

    template<typename T, typename D>
    bool operator<=(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return x.value() <= y.value();
    }

    template<typename T, typename D>
    bool operator>(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return x.value() > y.value();
    }

    template<typename T, typename D>
    bool operator>=(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return x.value() >= y.value();
    }

    //----------------------------------------------------------------
    // Arithmetic operators
    //----------------------------------------------------------------

    template<typename T, typename D>
    quantity<T, D> operator+(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return quantity<T, D>(x) += y;
    }

    template<typename T, typename D>
    quantity<T, D> operator-(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return quantity<T, D>(x) -= y;
    }

    template<typename T, typename D>
    quantity<T, D> operator*(quantity<T, D> const& x, typename quantity<T, D>::value_type a) noexcept
    {
        return quantity<T, D>(x) *= a;
    }

    template<typename T, typename D>
    quantity<T, D> operator*(typename quantity<T, D>::value_type a, quantity<T, D> const& x) noexcept
    {
        return quantity<T, D>(x) *= a;
    }

    template<typename T, typename D>
    quantity<T, D> operator/(quantity<T, D> const& x, typename quantity<T, D>::value_type a)
    {
        return quantity<T, D>(x) /= a;
    }

    //----------------------------------------------------------------
    // Dimension-correct arithmetics
    //----------------------------------------------------------------

    template<typename T, typename DX, typename DY,
             typename D = typename arithmetic_traits<DX, DY>::product_dimension>
    quantity<T, D> operator*(quantity<T, DX> const& x, quantity<T, DY> const& y) noexcept
    {
        return quantity<T, D>{x.value() * y.value()};
    }

    template<typename T, typename DX, typename DY,
             typename D = typename arithmetic_traits<DX, DY>::quotient_dimension>
    quantity_or_scalar<T, D> operator/(quantity<T, DX> const& x, quantity<T, DY> const& y)
    {
        return quantity_or_scalar<T, D>{x.value() / y.value()};
    }

    template<typename T, typename D,
             typename RD = typename dimension_traits<D>::template power<-1>>
    quantity<T, RD> operator/(typename quantity<T, D>::value_type a, quantity<T, D> const& x)
    {
        return quantity<T, RD>{a} /= x.value();
    }

    //----------------------------------------------------------------
    // Math operations
    //----------------------------------------------------------------

    /*
     * Computes the x raised to N-th power.
     */
    template<int N>
    double pow(double x)
    {
        return std::pow(x, N);
    }

    template<int N>
    float pow(float x)
    {
        return std::pow(x, N);
    }

    /*
     * Computes the x raised to N-th power with correct dimensions.
     */
    template<int N, typename T, typename D,
             typename RD = typename dimension_traits<D>::template power<N>>
    quantity_or_scalar<T, RD> pow(quantity<T, D> const& x)
    {
        return quantity_or_scalar<T, RD>{pow<N>(x.value())};
    }

    /*
     * Computes the square root of x.
     */
    inline
    double sqrt(double x)
    {
        return std::sqrt(x);
    }

    inline
    float sqrt(float x)
    {
        return std::sqrt(x);
    }

    /*
     * Computes the square root of x with correct dimensions.
     */
    template<typename T, typename D,
             typename RD = typename dimension_traits<D>::template root<2>>
    quantity<T, RD> sqrt(quantity<T, D> const& x)
    {
        return quantity<T, RD>{sqrt(x.value())};
    }

    /*
     * Computes the cubic root of x.
     */
    inline
    double cbrt(double x) noexcept
    {
        return std::cbrt(x);
    }

    inline
    float cbrt(float x) noexcept
    {
        return std::cbrt(x);
    }

    /*
     * Computes the cubic root of x with correct dimensions.
     */
    template<typename T, typename D,
             typename RD = typename dimension_traits<D>::template root<3>>
    quantity<T, RD> cbrt(quantity<T, D> const& x) noexcept
    {
        return quantity<T, RD>{cbrt(x.value())};
    }

    /*
     * Computes the square of x.
     */
    inline
    double square(double x) noexcept
    {
        return x * x;
    }

    inline
    float square(float x) noexcept
    {
        return x * x;
    }

    /*
     * Computes the square of x with correct dimensions.
     */
    template<typename T, typename D,
             typename RD = typename dimension_traits<D>::template power<2>>
    quantity<T, RD> square(quantity<T, D> const& x) noexcept
    {
        return x * x;
    }

    /*
     * Computes the inverse of x.
     */
    inline
    double inverse(double x)
    {
        return double(1) / x;
    }

    inline
    float inverse(float x)
    {
        return float(1) / x;
    }

    /*
     * Computes the inverse of x with correct dimensions.
     */
    template<typename T, typename D,
             typename RD = typename dimension_traits<D>::template power<-1>>
    quantity<T, RD> inverse(quantity<T, D> const& x)
    {
        return quantity<T, RD>{inverse(x.value())};
    }

    /*
     * Computes the absolute value.
     */
    inline
    double abs(double x) noexcept
    {
        return std::fabs(x);
    }

    inline
    float abs(float x) noexcept
    {
        return std::fabs(x);
    }

    /*
     * Computes the absolute value with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> abs(quantity<T, D> const& x) noexcept
    {
        return quantity<T, D>{abs(x.value())};
    }

    /*
     * Computes the square root of the sum of the squares of x and y.
     */
    inline
    double hypot(double x, double y) noexcept
    {
        return std::hypot(x, y);
    }

    inline
    float hypot(float x, float y) noexcept
    {
        return std::hypot(x, y);
    }

    /*
     * Computes the square root of the sum of the squares of x and y with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> hypot(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return quantity<T, D>{hypot(x.value(), y.value())};
    }

    /*
     * Computes the square root of the sum of the squares of x, y and z.
     */
    inline
    double hypot(double x, double y, double z) noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline
    float hypot(float x, float y, float z) noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    /*
     * Computes the square root of the sum of the squares of x, y and z with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> hypot(quantity<T, D> const& x, quantity<T, D> const& y, quantity<T, D> const& z) noexcept
    {
        return quantity<T, D>{hypot(x.value(), y.value(), z.value())};
    }

    //----------------------------------------------------------------
    // Standard dimension
    //----------------------------------------------------------------

    template<int Length, int Mass, int Time>
    struct mechanical_dimension
    {
    };

    template<int L, int M, int T>
    struct dimension_traits<mechanical_dimension<L, M, T>>
    {
        static constexpr bool is_dimensionless = (L == 0 && M == 0 && T == 0);

        template<int N>
        using power = mechanical_dimension<L * N, M * N, T * N>;

        template<int N>
        using root = typename std::enable_if<
            (L % N == 0 && M % N == 0 && T % N == 0),
            mechanical_dimension<L / N, M / N, T / N>
        >::type;

    };

    template<int L1, int M1, int T1, int L2, int M2, int T2>
    struct arithmetic_traits<mechanical_dimension<L1, M1, T1>, mechanical_dimension<L2, M2, T2>>
    {
        using product_dimension = mechanical_dimension<L1 + L2, M1 + M2, T1 + T2>;
        using quotient_dimension = mechanical_dimension<L1 - L2, M1 - M2, T1 - T2>;
    };

    namespace dim
    {
        using length       = mechanical_dimension<1, 0, 0>;
        using area         = mechanical_dimension<2, 0, 0>;
        using volume       = mechanical_dimension<3, 0, 0>;
        using mass         = mechanical_dimension<0, 1, 0>;
        using time         = mechanical_dimension<0, 0, 1>;
        using speed        = mechanical_dimension<1, 0, -1>;
        using momentum     = mechanical_dimension<1, 1, -1>;
        using acceleration = mechanical_dimension<1, 0, -2>;
        using force        = mechanical_dimension<1, 1, -2>;
        using energy       = mechanical_dimension<2, 1, -2>;

    } // namespace dim

    using scalar       = double;
    using length       = quantity<scalar, dim::length>;
    using area         = quantity<scalar, dim::area>;
    using volume       = quantity<scalar, dim::volume>;
    using mass         = quantity<scalar, dim::mass>;
    using time         = quantity<scalar, dim::time>;
    using speed        = quantity<scalar, dim::speed>;
    using momentum     = quantity<scalar, dim::momentum>;
    using acceleration = quantity<scalar, dim::acceleration>;
    using force        = quantity<scalar, dim::force>;
    using energy       = quantity<scalar, dim::energy>;

} // namespace units

//----------------------------------------------------------------

#include <iostream>

int main()
{
    units::length disp{123};
    units::time dur{10};
    units::mass mass{1};

    units::speed spd = disp / dur;
    std::cout << "Speed: " << spd.value () << '\n';

    units::energy kin = units::square(spd) * mass / 2;
    std::cout << "Kinetic energy: " << kin.value () << '\n';

    units::energy ref{12.3};
    units::scalar ratio = units::abs(kin / ref);
    std::cout << "Ratio: " << ratio << '\n';
}
