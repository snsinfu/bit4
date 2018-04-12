#include <cstddef>

#include <xtensor/xshape.hpp>
#include <xtensor/xtensor.hpp>

#include "catch.hpp"
#include "math.hpp"


TEST_CASE("log_beta reduces the last dimension")
{
    xt::static_shape<std::size_t, 3> const input_shape = {3, 4, 5};
    xt::static_shape<std::size_t, 2> const output_shape = {3, 4};
    xt::xtensor<double, 3> const input{input_shape};
    CHECK(log_beta(input).shape() == output_shape);
}

TEST_CASE("log_beta computes correct value within < 1 percent error")
{
    xt::xtensor<double, 2> const input = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    xt::xtensor<double, 1> const expected = {
        -4.094345,
        -15.433916,
        -25.897660,
    };

    auto const output = log_beta(input);
    auto const rel_error = xt::abs((output - expected) / expected);
    auto const max_rel_error = xt::amax(rel_error)();

    CHECK(max_rel_error < 0.01);
}

TEST_CASE("log_beta works with expression rvalues")
{
    xt::xtensor<double, 2> const x1{xt::static_shape<std::size_t, 2>{2, 3}, 1};
    xt::xtensor<double, 2> const x2{xt::static_shape<std::size_t, 2>{2, 3}, 2};
    xt::xtensor<double, 2> const x1x2_sum = x1 + x2;

    auto const expected = log_beta(x1x2_sum);
    auto const output = log_beta(x1 + x2);
    auto const rel_error = xt::abs((output - expected) / expected);
    auto const max_rel_error = xt::amax(rel_error)();

    CHECK(max_rel_error < 1e-6);
}

TEST_CASE("digamma works element-wise")
{
    xt::xtensor<double, 2> const input{xt::static_shape<std::size_t, 2>{2, 3}, 1};
    CHECK(digamma(input).shape() == input.shape());
}

TEST_CASE("digamma computes correct value within < 1 percent error")
{
    xt::xtensor<double, 2> const input = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9},
    };

    xt::xtensor<double, 2> const expected = {
        {-0.577216, 0.422784, 0.922784},
        { 1.256118, 1.506118, 1.706118},
        { 1.872784, 2.015641, 2.140641},
    };

    auto const output = digamma(input);
    auto const rel_error = xt::abs((output - expected) / expected);
    auto const max_rel_error = xt::amax(rel_error)();

    CHECK(max_rel_error < 0.01);
}

TEST_CASE("digamma works with expression rvalues")
{
    xt::xtensor<double, 1> const x1{xt::static_shape<std::size_t, 1>{2}, 1};
    xt::xtensor<double, 1> const x2{xt::static_shape<std::size_t, 1>{2}, 2};
    xt::xtensor<double, 1> const x1x2_sum = x1 + x2;

    auto const expected = digamma(x1x2_sum);
    auto const output = digamma(x1 + x2);
    auto const rel_error = xt::abs((output - expected) / expected);
    auto const max_rel_error = xt::amax(rel_error)();

    CHECK(max_rel_error < 1e-6);
}
