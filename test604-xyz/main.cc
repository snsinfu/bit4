#include <cmath>
#include <cstddef>
#include <iostream>
#include <istream>
#include <ostream>


namespace geo
{
    enum
    {
        dimension = 3
    };

    struct coordinates
    {
        double x = 0;
        double y = 0;
        double z = 0;

        coordinates() = default;

        coordinates(double x, double y, double z)
            : x{x}, y{y}, z{z}
        {
        }

        inline std::size_t size() const noexcept
        {
            return dimension;
        }

        inline double& operator[](std::size_t i)
        {
            // Compilers are smart enough to optimize this construct to a code
            // that is equivalent to the non-portable hack: *(&x + i).
            double* coords[] {
                &x,
                &y,
                &z
            };
            return *coords[i];
        }

        inline double const& operator[](std::size_t i) const
        {
            double const* coords[] {
                &x,
                &y,
                &z
            };
            return *coords[i];
        }
    };

    template<typename Char, typename Tr>
    std::basic_istream<Char, Tr>& operator>>(
        std::basic_istream<Char, Tr>& is,
        coordinates& coords
    )
    {
        using sentry_type = typename std::basic_istream<Char, Tr>::sentry;

        if (sentry_type sentry{is}) {
            for (std::size_t i = 0; i < coords.size(); i++) {
                is >> coords[i];
            }
        }

        return is;
    }

    template<typename Char, typename Tr>
    std::basic_ostream<Char, Tr>& operator<<(
        std::basic_ostream<Char, Tr>& os,
        coordinates const& coords
    )
    {
        using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

        if (sentry_type sentry{os}) {
            Char const delim = os.widen(' ');

            for (std::size_t i = 0; i < coords.size(); i++) {
                if (i != 0) {
                    os << delim;
                }
                os << coords[i];
            }
        }

        return os;
    }

    struct vector : coordinates
    {
        using coordinates::coordinates;

        double dot(vector const& v) const
        {
            double sum = 0;

            for (std::size_t i = 0; i < dimension; i++) {
                sum += (*this)[i] * v[i];
            }

            return sum;
        }

        double squared_norm() const
        {
            return dot(*this);
        }

        double norm() const
        {
            return std::sqrt(squared_norm());
        }
    };

    vector operator-(vector const& va, vector const& vb)
    {
        vector v;

        for (std::size_t i = 0; i < dimension; i++) {
            v[i] = va[i] - vb[i];
        }

        return v;
    }
}

__attribute__((noinline))
double distance(geo::vector va, geo::vector vb)
{
    return (va - vb).norm();
}

int main()
{
    geo::vector va = {2, 4, 8};
    geo::vector vb = {3, 5, 7};
    std::cout << distance(va, vb) << '\n';
}
