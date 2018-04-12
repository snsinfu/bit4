#include <xtensor/xtensor.hpp>

#include "ijk.hpp"
#include "catch.hpp"


TEST_CASE("ij::i example")
{
    xt::xtensor<double, 1> const input = {
        1,
        2,
        3,
    };
    xt::xtensor<double, 2> const base = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    };
    xt::xtensor<double, 2> const expected = {
        {1, 1, 1},
        {2, 2, 2},
        {3, 3, 3},
    };
    CHECK((base + ij::i(input) == expected));
}

TEST_CASE("ijk::ij example")
{
    xt::xtensor<double, 2> const input = {
        {1, 2},
        {3, 4},
    };
    xt::xtensor<double, 3> const base = {
        {{0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}},
    };
    xt::xtensor<double, 3> const expected = {
        {{1, 1, 1}, {2, 2, 2}},
        {{3, 3, 3}, {4, 4, 4}},
    };
    CHECK((base + ijk::ij(input) == expected));
}

TEST_CASE("ijk::ji example")
{
    xt::xtensor<double, 2> const input = {
        {1, 2},
        {3, 4},
    };
    xt::xtensor<double, 3> const base = {
        {{0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}},
    };
    xt::xtensor<double, 3> const expected = {
        {{1, 1, 1}, {3, 3, 3}},
        {{2, 2, 2}, {4, 4, 4}},
    };
    CHECK((base + ijk::ji(input) == expected));
}

TEST_CASE("ijk::ik example")
{
    xt::xtensor<double, 2> const input = {
        {1, 2},
        {3, 4},
    };
    xt::xtensor<double, 3> const base = {
        {{0, 0}, {0, 0}},
        {{0, 0}, {0, 0}},
    };
    xt::xtensor<double, 3> const expected = {
        {{1, 2}, {1, 2}},
        {{3, 4}, {3, 4}}
    };
    CHECK((base + ijk::ik(input) == expected));
}

TEST_CASE("ijk::kj example")
{
    xt::xtensor<double, 2> const input = {
        {1, 2},
        {3, 4},
    };
    xt::xtensor<double, 3> const base = {
        {{0, 0}, {0, 0}},
        {{0, 0}, {0, 0}},
    };
    xt::xtensor<double, 3> const expected = {
        {{1, 3}, {2, 4}},
        {{1, 3}, {2, 4}},
    };
    CHECK((base + ijk::kj(input) == expected));
}
