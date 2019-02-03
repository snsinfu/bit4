#ifndef INCLUDED_VEC_HPP
#define INCLUDED_VEC_HPP

#include <cmath>


// Bare minimum 2D vector.
struct vec
{
    double x = 0;
    double y = 0;
};

inline vec operator-(vec const& v1, vec const& v2)
{
    return vec{v1.x - v2.x, v1.y - v2.y};
}

inline double squared_norm(vec const& v)
{
    return v.x * v.x + v.y * v.y;
}

inline double norm(vec const& v)
{
    return std::sqrt(squared_norm(v));
}

#endif
