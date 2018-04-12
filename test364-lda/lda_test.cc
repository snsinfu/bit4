#include <cstddef>

#include <xtensor/xtensor.hpp>

#include "catch.hpp"
#include "lda.hpp"


TEST_CASE("latent_dirichlet_allocation works with default config")
{
    latent_dirichlet_allocation::config default_config;
    latent_dirichlet_allocation lda{default_config};

    xt::xtensor<double, 2> const data = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 0, 1, 2},
        {3, 4, 5, 6},
        {7, 8, 9, 0},
    };
    lda.fit(data);

    CHECK(lda.score(data) <= 0); // log-likelihood is always non-positive
}
