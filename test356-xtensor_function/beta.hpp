#ifndef INCLUDED_BETA_HPP
#define INCLUDED_BETA_HPP

#include <xtensor/xmath.hpp>

#include "expr.hpp"


template<typename E>
auto log_beta(E&& e)
{
    return xt::sum(xt::lgamma(copy_expr(e)), {1}) - xt::lgamma(xt::sum(copy_expr(e), {1}));
}

#endif
