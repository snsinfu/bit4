#ifndef INCLUDED_MATH_HPP
#define INCLUDED_MATH_HPP

#include <cmath>


// Computes fmod(x, m) quickly. m_inv must be the inverse of m.
inline double fastmod(double x, double m, double m_inv)
{
    return x - std::floor(x * m_inv) * m;
}

// Truncates non-negative value x to unsigned integer. On x86-64 this is faster
// than just casting x to unsigned.
inline unsigned fasttrunc_uint(double x)
{
    // On x86-64 float-to-unsigned conversion is slow.
    return static_cast<unsigned>(static_cast<int>(x));
}

#endif
