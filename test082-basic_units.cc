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

    template<typename T>
    class quantity_store
    {
      public:
        quantity_store() = default;

        explicit quantity_store(T value)
            : value_{value}
        {
        }

      protected:
        T& value() noexcept
        {
            return value_;
        }

        T const& value() const noexcept
        {
            return value_;
        }

      private:
        T value_;
    };

    template<typename T, typename D, bool Allow = dimension_traits<D>::is_dimensionless>
    class quantity_conversion_policy;

    template<typename T, typename D>
    class quantity_conversion_policy<T, D, true> : public quantity_store<T>
    {
      public:
        quantity_conversion_policy() = default;

        quantity_conversion_policy(T value)
            : quantity_store<T>{value}
        {
        }

        operator T() const noexcept
        {
            return quantity_store<T>::value();
        }
    };

    template<typename T, typename D>
    class quantity_conversion_policy<T, D, false> : public quantity_store<T>
    {
      public:
        quantity_conversion_policy() = default;

        explicit quantity_conversion_policy(T value)
            : quantity_store<T>{value}
        {
        }
    };

    /*
     * Scalar quantity having type T and physical dimension D.
     */
    template<typename T, typename D>
    class quantity : public quantity_conversion_policy<T, D>
    {
      public:
        using quantity_conversion_policy<T, D>::quantity_conversion_policy;

        /*
         * The arithmetic type.
         */
        using value_type = T;

        /*
         * The dimension type.
         */
        using dimension = D;

        /*
         * Returns the numerical value without dimension.
         */
        value_type value() const noexcept
        {
            return quantity_store<T>::value();
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
            return quantity{-quantity_store<T>::value()};
        }

        quantity& operator+=(quantity const& rhs) noexcept
        {
            quantity_store<T>::value() += rhs.value();
            return *this;
        }

        quantity& operator-=(quantity const& rhs) noexcept
        {
            quantity_store<T>::value() -= rhs.value();
            return *this;
        }

        quantity& operator*=(value_type rhs) noexcept
        {
            quantity_store<T>::value() *= rhs;
            return *this;
        }

        quantity& operator/=(value_type rhs)
        {
            quantity_store<T>::value() /= rhs;
            return *this;
        }

      private:
        value_type value_;
    };

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
    quantity<T, D> operator*(
        quantity<T, D> const& x, typename quantity<T, D>::value_type a) noexcept
    {
        return quantity<T, D>(x) *= a;
    }

    template<typename T, typename D>
    quantity<T, D> operator*(
        typename quantity<T, D>::value_type a, quantity<T, D> const& x) noexcept
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
    quantity<T, D> operator/(quantity<T, DX> const& x, quantity<T, DY> const& y)
    {
        return quantity<T, D>{x.value() / y.value()};
    }

    template<typename T, typename D, typename RD = typename dimension_traits<D>::template power<-1>>
    quantity<T, RD> operator/(typename quantity<T, D>::value_type a, quantity<T, D> const& x)
    {
        return quantity<T, RD>{a} /= x.value();
    }

    //----------------------------------------------------------------
    // Math operations
    //----------------------------------------------------------------

    /*
     * Computes the x raised to N-th power with correct dimensions.
     */
    template<int N, typename T, typename D,
        typename RD = typename dimension_traits<D>::template power<N>>
    quantity<T, RD> pow(quantity<T, D> const& x)
    {
        return quantity<T, RD>{std::pow(x.value(), N)};
    }

    /*
     * Computes the square root of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = typename dimension_traits<D>::template root<2>>
    quantity<T, RD> sqrt(quantity<T, D> const& x)
    {
        return quantity<T, RD>{std::sqrt(x.value())};
    }

    /*
     * Computes the cubic root of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = typename dimension_traits<D>::template root<3>>
    quantity<T, RD> cbrt(quantity<T, D> const& x) noexcept
    {
        return quantity<T, RD>{std::cbrt(x.value())};
    }

    /*
     * Computes the square of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = typename dimension_traits<D>::template power<2>>
    quantity<T, RD> square(quantity<T, D> const& x) noexcept
    {
        return x * x;
    }

    /*
     * Computes the inverse of x with correct dimensions.
     */
    template<typename T, typename D, typename RD = typename dimension_traits<D>::template power<-1>>
    quantity<T, RD> inverse(quantity<T, D> const& x)
    {
        return quantity<T, RD>{T(1) / x.value()};
    }

    /*
     * Computes the absolute value with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> abs(quantity<T, D> const& x) noexcept
    {
        return quantity<T, D>{std::fabs(x.value())};
    }

    /*
     * Computes the square root of the sum of the squares of x and y with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> hypot(quantity<T, D> const& x, quantity<T, D> const& y) noexcept
    {
        return quantity<T, D>{std::hypot(x.value(), y.value())};
    }

    /*
     * Computes the square root of the sum of the squares of x, y and z with correct dimensions.
     */
    template<typename T, typename D>
    quantity<T, D> hypot(
        quantity<T, D> const& x, quantity<T, D> const& y, quantity<T, D> const& z) noexcept
    {
        return quantity<T, D>{std::hypot(x.value(), y.value(), z.value())};
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
        using root = typename std::enable_if<(L % N == 0 && M % N == 0 && T % N == 0),
            mechanical_dimension<L / N, M / N, T / N>>::type;
    };

    template<int L1, int M1, int T1, int L2, int M2, int T2>
    struct arithmetic_traits<mechanical_dimension<L1, M1, T1>, mechanical_dimension<L2, M2, T2>>
    {
        using product_dimension = mechanical_dimension<L1 + L2, M1 + M2, T1 + T2>;
        using quotient_dimension = mechanical_dimension<L1 - L2, M1 - M2, T1 - T2>;
    };

    namespace dim
    {
        using scalar = mechanical_dimension<0, 0, 0>;
        using length = mechanical_dimension<1, 0, 0>;
        using area = mechanical_dimension<2, 0, 0>;
        using volume = mechanical_dimension<3, 0, 0>;
        using mass = mechanical_dimension<0, 1, 0>;
        using time = mechanical_dimension<0, 0, 1>;
        using speed = mechanical_dimension<1, 0, -1>;
        using momentum = mechanical_dimension<1, 1, -1>;
        using acceleration = mechanical_dimension<1, 0, -2>;
        using force = mechanical_dimension<1, 1, -2>;
        using energy = mechanical_dimension<2, 1, -2>;
    } // namespace dim

    using scalar = quantity<double, dim::scalar>;
    using length = quantity<double, dim::length>;
    using area = quantity<double, dim::area>;
    using volume = quantity<double, dim::volume>;
    using mass = quantity<double, dim::mass>;
    using time = quantity<double, dim::time>;
    using speed = quantity<double, dim::speed>;
    using momentum = quantity<double, dim::momentum>;
    using acceleration = quantity<double, dim::acceleration>;
    using force = quantity<double, dim::force>;
    using energy = quantity<double, dim::energy>;
} // namespace units

//----------------------------------------------------------------

#include <iostream>

int main()
{
    units::length disp{123};
    units::time dur{10};
    units::mass mass{1};

    units::speed spd = disp / dur;
    std::cout << "Speed: " << spd.value() << '\n';

    units::energy kin = units::square(spd) * mass / 2;
    std::cout << "Kinetic energy: " << kin.value() << '\n';

    units::energy ref{12.3};
    units::scalar ratio = units::abs(kin / ref);
    std::cout << "Ratio: " << ratio << '\n';

    // Implicit conversion between scalar <-> double
    double ratio_double = ratio;
    ratio = ratio_double;
    ratio = std::sin(ratio);
}
