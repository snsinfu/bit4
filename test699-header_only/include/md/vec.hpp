#ifndef MD_VEC_HPP
#define MD_VEC_HPP

namespace md
{
    struct vec
    {
        double x = 0;
        double y = 0;
        double z = 0;

        // dot returns the dot product of this vector and v.
        double dot(md::vec const& v) const;

        // norm returns the Euclidean norm of this vector.
        double norm() const;
    };

    // Subtraction `v1 + v2` computes a vector consisting of the component-wise
    // sum of v1 and v2.
    md::vec operator+(md::vec const& v1, md::vec const& v2);

    // Subtraction `v1 - v2` computes a vector consisting of the component-wise
    // difference of v1 and v2.
    md::vec operator-(md::vec const& v1, md::vec const& v2);

    // Scalar multiplication `a * v` broadcasts to vector components.
    md::vec operator*(double a, md::vec const& v);
}

#endif
