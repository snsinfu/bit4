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
    xt::xtensor<double, 2> X({10000, 6});

    for (std::size_t i = 0; i < X.shape()[0]; ++i) {
        for (std::size_t j = 0; j < X.shape()[1]; ++j) {
            X(i, j) = ((i % X.shape()[1]) == j) * 5.0;
        }
    }
    X += xt::random::rand<double>(X.shape());

    latent_dirichlet_allocation::config config;
    config.topic_count = 3;
    config.outer_step_count = 60;
    config.inner_step_count = 30;

    config.topic_word_preconditions = xt::xtensor<double, 2>{
        {1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 1}
    };
    config.topic_word_preconditions += 100;
    config.topic_word_preconditions /= 100;

    latent_dirichlet_allocation lda{config};

    lda.train(X);

    std::cout << "Topic-word distribution:\n"
              << lda.topic_word_distribution()
              << '\n';

    std::cout << "Doc-topic distribution:\n"
              << lda.doc_topic_distribution()
              << '\n';
}
