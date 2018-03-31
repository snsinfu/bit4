#include <xtensor/xrandom.hpp>
#include <xtensor/xstrided_view.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

int main()
{
    xt::xtensor<double, 2> X({10, 20});
    xt::xtensor<double, 2> Y(X.shape());

    X = xt::random::randn<double>(X.shape());

    // OK
    Y = xt::view(xt::sum(X, {1}), xt::all(), xt::newaxis());

    /*
    // Error: no member named 'begin' in 'xt::detail::expression_adaptor...'
    Y = xt::expand_dims(xt::sum(X, {1}), 1);
    */
}

// xt::view is static and xt::dynamic_view (returned from xt::expand_dims) is
// dynamic. I guess this difference has something to do with the error.
