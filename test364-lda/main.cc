#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

#include "lda.hpp"


// Normalizes the last dimension.
xt::xtensor<double, 2> normalize(xt::xtensor<double, 2> const& distrs)
{
    return distrs / xt::view(xt::sum(distrs, {1}), xt::all(), xt::newaxis());
}

int main()
{
    xt::xtensor<double, 2> const X = {
        {10, 5, 0,  0, 0},
        { 9, 4, 1,  0, 0},
        {15, 3, 0,  1, 0},
        { 2, 1, 1,  5, 0},
        { 1, 1, 2,  4, 0},
        { 2, 0, 1,  8, 1},
        {10, 3, 4, 90, 5},
    };

    latent_dirichlet_allocation::config config;
    config.doc_topic_prior = 1.0;
    config.topic_word_prior = 0.1;
    config.topic_word_preconditions = {
        {1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
    };
    config.topic_count = config.topic_word_preconditions.shape()[0];

    latent_dirichlet_allocation lda{config};

    lda.fit(X);

    xt::print_options::set_precision(2);

    std::cout << "Score:\n"
              << lda.score(X) << '\n';

    std::cout << "\nTopic-word distribution:\n"
              << normalize(lda.topic_word_distribution()) << '\n';

    std::cout << "\nDocument-topic distribution:\n"
              << normalize(lda.transform(X)) << '\n';
}
