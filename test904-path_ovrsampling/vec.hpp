#pragma once

#include <cmath>


struct vec
{
    double x = 0;
    double y = 0;
    double z = 0;
};


double dot(vec v1, vec v2);
vec    normalize(vec v);
vec    operator+(vec v1, vec v2);
vec    operator-(vec v1, vec v2);
vec    operator*(double a, vec v);


inline double dot(vec v1, vec v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


inline vec normalize(vec v)
{
    return 1 / std::sqrt(dot(v, v)) * v;
}


inline vec operator+(vec v1, vec v2)
{
    return vec{v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}


inline vec operator-(vec v1, vec v2)
{
    return vec{v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}


inline vec operator*(double a, vec v)
{
    return vec{a * v.x, a * v.y, a * v.z};
}
