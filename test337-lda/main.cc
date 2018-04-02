#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xtensor.hpp>

#include "lda.hpp"


int main()
{
    latent_dirichlet_allocation::config config;
    config.topic_count = 5;
    config.e_step_count = 20;
    config.m_step_count = 20;

    latent_dirichlet_allocation lda{config};

    xt::xtensor<double, 2> X({30000, 6});

    for (std::size_t i = 0; i < X.shape()[0]; ++i) {
        for (std::size_t j = 0; j < X.shape()[1]; ++j) {
            X(i, j) = ((i % 6) == j) * 5.0;
        }
    }
    X += xt::random::rand<double>(X.shape());

    lda.fit(X);

    std::cout << lda.topic_word_distribution() << '\n';
}
