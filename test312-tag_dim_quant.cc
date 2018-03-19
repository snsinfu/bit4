// Type-tagged dimensional analysis

namespace qu
{
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
}

namespace qu
{
    namespace detail
    {
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

            scalar_mixin(T value) // NOLINT: Allow implicit conversion
                : value_{value}
            {
            }

            operator T() const // NOLINT: Allow implicit conversion
            {
                return value_;
            }

          protected:
            T value_{};
        };
    }

    template<typename T, typename D, typename Tag = void>
    class scalar : private detail::scalar_mixin<T, D>
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

        scalar& operator*=(number_type rhs)
        {
            value_ *= rhs;
            return *this;
        }

        scalar& operator/=(number_type rhs)
        {
            value_ /= rhs;
            return *this;
        }
    };

    template<typename T, typename D, typename Tag>
    bool operator==(scalar<T, D, Tag> const& lhs, scalar<T, D, Tag> const& rhs)
    {
        return lhs.value() == rhs.value();
    }

    template<typename T, typename D, typename Tag>
    bool operator!=(scalar<T, D, Tag> const& lhs, scalar<T, D, Tag> const& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename D, typename Tag>
    scalar<T, D, Tag> operator+(scalar<T, D, Tag> const& scal)
    {
        return scal;
    }

    template<typename T, typename D, typename Tag>
    scalar<T, D, Tag> operator-(scalar<T, D, Tag> const& scal)
    {
        return {scal.value()};
    }

    template<typename T, typename D, typename Tag1, typename Tag2>
    scalar<T, D> operator+(scalar<T, D, Tag1> const& lhs,
                           scalar<T, D, Tag2> const& rhs)
    {
        return {lhs.value() + rhs.value()};
    }

    template<typename T, typename D, typename Tag1, typename Tag2>
    scalar<T, D> operator-(scalar<T, D, Tag1> const& lhs,
                           scalar<T, D, Tag2> const& rhs)
    {
        return {lhs.value() - rhs.value()};
    }
}

namespace qu
{
    template<int L, int M, int T, int C>
    struct mechanical_dimension
    {
    };

    template<int L, int M, int T, int C>
    struct dimension_traits<mechanical_dimension<L, M, T, C>>
    {
        static constexpr bool is_zero = (L == 0 && M == 0 && T == 0 && C == 0);
    };

    template<int L1, int M1, int T1, int C1, int L2, int M2, int T2, int C2>
    struct product_dimension<mechanical_dimension<L1, M1, T1, C1>,
                             mechanical_dimension<L2, M2, T2, C2>>
    {
        using type = mechanical_dimension<L1 + L2, M1 + M2, T1 + T2, C1 + C2>;
    };

    template<int L1, int M1, int T1, int C1, int L2, int M2, int T2, int C2>
    struct quotient_dimension<mechanical_dimension<L1, M1, T1, C1>,
                              mechanical_dimension<L2, M2, T2, C2>>
    {
        using type = mechanical_dimension<L1 - L2, M1 - M2, T1 - T2, C1 - C2>;
    };

    template<int L, int M, int T, int C, int N>
    struct power_dimension<mechanical_dimension<L, M, T, C>, N>
    {
        using type = mechanical_dimension<L * N, M * N, T * N, C * N>;
    };

    template<int L, int M, int T, int C, int N>
    struct root_dimension<mechanical_dimension<L, M, T, C>, N>
    {
        static_assert(
            L % N == 0 && M % N == 0 && T % N == 0 && C % N == 0,
            "fractional dimension is not supported");
        using type = mechanical_dimension<L / N, M / N, T / N, C / N>;
    };

    namespace dim
    {
        using number       = mechanical_dimension<0, 0, 0, 0>;
        using length       = mechanical_dimension<1, 0, 0, 0>;
        using mass         = mechanical_dimension<0, 1, 0, 0>;
        using time         = mechanical_dimension<0, 0, 1, 0>;
        using charge       = mechanical_dimension<0, 0, 0, 1>;
        using speed        = quotient_dimension_t<length, time>;
        using acceleration = quotient_dimension_t<speed, time>;
        using momentum     = product_dimension_t<speed, mass>;
        using force        = product_dimension_t<acceleration, mass>;
        using energy       = product_dimension_t<force, length>;
    }
}

int main()
{
    using freq_dim = qu::power_dimension_t<qu::dim::time, -1>;

    // Frequency and damping coefficnet have the same dimension with differing
    // semantics.
    using frequency = qu::scalar<double, freq_dim, struct frequnecy_tag>;
    using damping_coefficient = qu::scalar<double, freq_dim, struct damping_coefficient_tag>;

    frequency w{1};
    damping_coefficient g{2};
    w + g;
    //frequency w2 = w + g;
}
