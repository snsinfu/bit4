#ifndef INCLUDED_POINT_HPP
#define INCLUDED_POINT_HPP

#include <cmath>
#include <cstddef>
#include <istream>
#include <ostream>


namespace md
{
    namespace point_detail
    {
        // coordinates hold three-dimensional cartesian coordinate values.
        struct coordinates
        {
            double x = 0;
            double y = 0;
            double z = 0;

            // Default constructor initializes coordinates to zero.
            coordinates() = default;

            // Constructor initializes coordinates to specified values.
            inline coordinates(double x_init, double y_init, double z_init)
                : x{x_init}, y{y_init}, z{z_init}
            {
            }

            // Index 0, 1 and 2 correspond to x, y and z, respectively.
            inline double& operator[](std::size_t idx)
            {
                double* coords[] = {
                    &x,
                    &y,
                    &z
                };
                return *coords[idx];
            }

            // Index 0, 1 and 2 correspond to x, y and z, respectively.
            inline double const& operator[](std::size_t idx) const
            {
                double const* coords[] = {
                    &x,
                    &y,
                    &z
                };
                return *coords[idx];
            }
        };

        // ostream operator formats coords as space-delimited floating-point
        // coordinate values.
        template<typename Char, typename Tr>
        std::basic_ostream<Char, Tr>& operator<<(
            std::basic_ostream<Char, Tr>& os,
            coordinates const& coords
        )
        {
            using sentry_type = typename std::basic_ostream<Char, Tr>::sentry;

            if (sentry_type sentry {os}) {
                Char const delim = os.widen(' ');
                os << coords.x << delim << coords.y << delim << coords.z;
            }

            return os;
        }

        // istream operator parses space-delimited floating-point coordinate
        // values into coords.
        template<typename Char, typename Tr>
        std::basic_istream<Char, Tr>& operator>>(
            std::basic_istream<Char, Tr>& is,
            coordinates& coords
        )
        {
            using sentry_type = typename std::basic_istream<Char, Tr>::sentry;

            if (sentry_type sentry {is}) {
                is >> coords.x >> coords.y >> coords.z;
            }

            return is;
        }
    }

    // vector is a three-dimensional vector with cartesian coordinates.
    struct vector : point_detail::coordinates
    {
        using point_detail::coordinates::coordinates;

        // Component-wise addition.
        inline vector& operator+=(vector const& other) noexcept
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Component-wise subtraction.
        inline vector& operator-=(vector const& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Scalar multiplication is broadcast to all components.
        inline vector& operator*=(double mult) noexcept
        {
            x *= mult;
            y *= mult;
            z *= mult;
            return *this;
        }

        // Scalar division is broadcast to all components.
        inline vector& operator/=(double divisor)
        {
            return *this *= 1 / divisor;
        }

        // dot returns the dot product of this vector and other.
        inline double dot(vector const& other) const noexcept
        {
            return (x * other.x + y * other.y + z * other.z);
        }

        // cross returns the cross product of this vector and rhs.
        inline vector cross(vector const& rhs) const noexcept
        {
            return vector {
                y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x
            };
        }

        // norm returns the Euclidean norm of this vector.
        inline double norm() const noexcept
        {
            return std::sqrt(squared_norm());
        }

        // squared_norm returns the squared Euclidean norm of this vector.
        inline double squared_norm() const noexcept
        {
            return dot(*this);
        }

        // normalize returns the unit vector that is parallel to this vector.
        // Behavior is undefined if this vector is zero.
        inline vector normalize() const
        {
            return vector{*this} /= norm();
        }
    };

    // Unary no-op `+vec` returns a copy of vec.
    inline vector operator+(vector const& vec) noexcept
    {
        return vec;
    }

    // Unary negation `-vec` returns an anti-parallel copy of vec.
    inline vector operator-(vector const& vec) noexcept
    {
        return {-vec.x, -vec.y, -vec.z};
    }

    // Sum `lhs + rhs` returns a vector with component-wise sum of lhs and rhs.
    inline vector operator+(vector const& lhs, vector const& rhs) noexcept
    {
        return vector{lhs} += rhs;
    }

    // Difference `lhs - rhs` returns a vector with component-wise difference of
    // lhs and rhs.
    inline vector operator-(vector const& lhs, vector const& rhs) noexcept
    {
        return vector{lhs} -= rhs;
    }

    // Multiplication `vec * mult` returns vec with each component scaled by
    // mult.
    inline vector operator*(vector const& vec, double mult) noexcept
    {
        return vector{vec} *= mult;
    }

    // Multiplication `mult * vec` returns vec with each component scaled by
    // mult.
    inline vector operator*(double mult, vector const& vec) noexcept
    {
        return vector{vec} *= mult;
    }

    // Division `vec / divisor` returns vec with each component scaled by
    // 1/divisor.
    inline vector operator/(vector const& vec, double divisor)
    {
        return vector{vec} /= divisor;
    }

    // dot returns the dot product of lhs and rhs.
    inline double dot(vector const& lhs, vector const& rhs) noexcept
    {
        return lhs.dot(rhs);
    }

    // cross returns the cross product of lhs and rhs.
    inline vector cross(vector const& lhs, vector const& rhs) noexcept
    {
        return lhs.cross(rhs);
    }

    // norm returns the Euclidean norm of vec.
    inline double norm(vector const& vec) noexcept
    {
        return vec.norm();
    }

    // squared_norm returns the squares Euclidean norm of vec.
    inline double squared_norm(vector const& vec) noexcept
    {
        return vec.squared_norm();
    }

    // normalize returns the unit vector that is parallel to vec.
    inline vector normalize(vector const& vec) noexcept
    {
        return vec.normalize();
    }

    // point is a cartesian representation of a point in the three-dimensional
    // Euclidean space.
    struct point : point_detail::coordinates
    {
        using point_detail::coordinates::coordinates;
        using vector_t = struct vector;

        // vector returns the coordinate vector of this point.
        inline vector_t vector() const noexcept
        {
            return vector_t {x, y, z};
        }

        // Adding a vector translates the point.
        inline point& operator+=(vector_t const& disp) noexcept
        {
            x += disp.x;
            y += disp.y;
            z += disp.z;
            return *this;
        }

        // Subtracting a vector translates the point.
        inline point& operator-=(vector_t const& disp) noexcept
        {
            x -= disp.x;
            y -= disp.y;
            z -= disp.z;
            return *this;
        }

        // distance returns the Euclidean distance between this point and other.
        inline double distance(point const& other) const noexcept
        {
            return (vector() - other.vector()).norm();
        }

        // squared_distance returns the squared Euclidean distance between this
        // point and other.
        inline double squared_distance(point const& other) const noexcept
        {
            return (vector() - other.vector()).squared_norm();
        }
    };

    // Sum `pt + disp` returns the point pt translated by disp.
    inline point operator+(point const& pt, vector const& disp) noexcept
    {
        return point {pt} += disp;
    }

    // Difference `pt - disp` returns the point pt translated by -disp.
    inline point operator-(point const& pt, vector const& disp) noexcept
    {
        return point {pt} -= disp;
    }

    // Difference `pa - pb` returns the displacement vector from pb towards pa.
    inline vector operator-(point const& pa, point const& pb) noexcept
    {
        return pa.vector() - pb.vector();
    }

    // distance returns the Euclidean distance between pa and pb.
    inline double distance(point const& pa, point const& pb) noexcept
    {
        return pa.distance(pb);
    }

    // squared_distance returns the squared Euclidean distance between pa and
    // pb.
    inline double squared_distance(point const& pa, point const& pb) noexcept
    {
        return pa.squared_distance(pb);
    }
}

#endif
