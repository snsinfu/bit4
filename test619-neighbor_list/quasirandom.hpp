#ifndef INCLUDED_QUASIRANDOM_HPP
#define INCLUDED_QUASIRANDOM_HPP

#include <array>
#include <cmath>


class kronecker_sequence
{
public:
    kronecker_sequence(double init, double alpha)
        : value_{init}, alpha_{alpha}
    {
    }

    double operator()()
    {
        value_ += alpha_;
        value_ -= std::floor(value_);
        return value_;
    }

private:
    double value_;
    double alpha_;
};


#endif
