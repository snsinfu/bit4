namespace sim
{
    template<typename T, typename Tag>
    struct sealed
    {
        explicit sealed(T value)
            : value_{value}
        {
        }

        T value() const
        {
            return value_;
        }

        sealed operator+() const
        {
            return *this;
        }

        sealed operator-() const
        {
            return sealed{-value_};
        }

        sealed& operator+=(sealed const& other)
        {
            value_ += other.value_;
            return *this;
        }

        sealed operator-=(sealed const& other)
        {
            value_ -= other.value_;
            return *this;
        }

        sealed operator*=(sealed const& other)
        {
            value_ *= other.value_;
            return *this;
        }

        sealed operator/=(sealed const& other)
        {
            value_ /= other.value_;
            return *this;
        }

        sealed operator%=(sealed const& other)
        {
            value_ %= other.value_;
            return *this;
        }

      private:
        T value_;
    };

    template<typename T, typename Tag>
    bool operator==(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return lhs.value() == rhs.value();
    }

    template<typename T, typename Tag>
    bool operator!=(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T, typename Tag>
    bool operator<(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return lhs.value() < rhs.value();
    }

    template<typename T, typename Tag>
    bool operator<=(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return lhs.value() <= rhs.value();
    }

    template<typename T, typename Tag>
    bool operator>(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return lhs.value() > rhs.value();
    }

    template<typename T, typename Tag>
    bool operator>=(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return lhs.value() >= rhs.value();
    }

    template<typename T, typename Tag>
    sealed<T, Tag> operator+(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return sealed<T, Tag>{lhs} += rhs;
    }

    template<typename T, typename Tag>
    sealed<T, Tag> operator-(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return sealed<T, Tag>{lhs} -= rhs;
    }

    template<typename T, typename Tag>
    sealed<T, Tag> operator*(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return sealed<T, Tag>{lhs} *= rhs;
    }

    template<typename T, typename Tag>
    sealed<T, Tag> operator/(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return sealed<T, Tag>{lhs} /= rhs;
    }

    template<typename T, typename Tag>
    sealed<T, Tag> operator%(sealed<T, Tag> const& lhs, sealed<T, Tag> const& rhs)
    {
        return sealed<T, Tag>{lhs} %= rhs;
    }
}

#include <iostream>
#include <type_traits>

int main()
{
    using quantity = sim::sealed<double, struct quantity_tag>;

    quantity p{123};
    quantity q{456};
    quantity r = p + q;

    r += p * q;
    r -= p / q;

    std::cout << r.value() << '\n';
}
