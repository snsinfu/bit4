#include <iostream>
#include <cstddef>
#include <vector>

#include <xtensor/xmath.hpp>
#include <xtensor/xnorm.hpp>
#include <xtensor/xreducer.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xstrided_view.hpp>

#include "catch.hpp"
#include "lda.hpp"
#include "testutil.hpp"


namespace
{
    // Computes the cosine similarity of two vectors.
    double cosine_similarity(xt::xtensor<double, 1> const& u,
                             xt::xtensor<double, 1> const& v)
    {
        auto const u_norm = u / xt::norm_l2(u);
        auto const v_norm = v / xt::norm_l2(v);
        return xt::sum(u_norm * v_norm)();
    }
}

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

TEST_CASE("latent_dirichlet_allocation consistent with sklearn")
{
    // Example document.
    xt::xtensor<double, 2> const data = {
        { 4,   3, 19, 59,   6},
        {12,   7, 45, 57,  14},
        {32,   4, 11,  5, 153},
        {22,   3, 31, 11,  47},
        { 1,   2, 79,  5,   4},
        {19,   4, 18, 13,  96},
        { 0,   3, 79,  4,   4},
        { 2,   1, 74,  6,   6},
        {19,   5, 33, 59,  14},
        {59, 324,  9, 14,  16},
        { 3,   4, 49, 14,   7},
        {67,   7, 10, 14,  10},
        {48,   4, 13, 10,  14},
        { 2,   1, 43, 10,   3},
        {63,   6, 12, 10,  72},
        {79,   7, 20, 21,  59},
        {20,   8, 22, 10, 328},
        {59,   5, 17, 40,  20},
        { 2,   2, 60,  7,   7},
        { 4,   3, 72,  3,   9},
    };

    // Topic-word and document-topic dirichlets estimated by sklearn's LDA.
    // The order of topics may not match ours because of randomness in the
    // training algorithm.
    xt::xtensor<double, 2> const sklearn_topics = {
        {384.0,  31.3,  72.1,  78.7,  88.6},
        { 19.2,  16.0,  73.0, 231.0,  35.4},
        { 45.3,  15.9,  44.5,  18.5, 708.0},
        { 10.0,  16.2, 518.0,  30.6,  41.4},
        { 59.1, 324.0,   9.1,  14.1,  16.1},
    };
    xt::xtensor<double, 2> const sklearn_docs = {
        {  0.1, 91.1,    0.1,  0.1,    0.1},
        { 12.3, 91.3,    0.1, 31.7,    0.1},
        { 33.1,  0.1,  172.0,  0.1,    0.1},
        { 32.3,  6.52,  47.3, 28.3,    0.1},
        {  0.1,  0.1,    0.1, 91.1,    0.1},
        { 21.4, 12.4,  108.0,  8.52,   0.1},
        {  0.1,  0.1,    0.1, 90.1,    0.1},
        {  0.1,  0.1,    0.1, 89.1,    0.1},
        { 24.5, 91.3,    0.1, 14.5,    0.1},
        {  0.1,  0.1,    0.1,  0.1,  422.0},
        {  0.1, 20.9,    0.1, 56.3,    0.1},
        {108.0,  0.1,    0.1,  0.1,    0.1},
        { 89.1,  0.1,    0.1,  0.1,    0.1},
        {  0.1, 11.8,    0.1, 47.4,    0.1},
        { 94.4,  0.1,   68.8,  0.1,    0.1},
        {138.0,  0.1,   47.8,  0.1,    0.1},
        {  0.1,  0.1,  388.0,  0.1,    0.1},
        { 97.4, 43.8,    0.1,  0.1,    0.1},
        {  0.1,  5.49,   0.1, 72.7,    0.1},
        {  4.2,  0.1,    0.1, 87.0,    0.1},
    };

    // Our estimate.

    latent_dirichlet_allocation::config config;
    config.topic_count = 5;
    config.doc_topic_prior = 0.1;
    config.topic_word_prior = 0.1;
    config.convergence_threshold = 1e-6;

    latent_dirichlet_allocation lda{config};
    lda.fit(data);

    xt::xtensor<double, 2> const topics = lda.topic_word_dirichlets();
    xt::xtensor<double, 2> const docs = lda.transform(data);

    // Test our estimate with the cosine similarity. Higher is better and the
    // similarity is 1 for exact match. The order of topics is properly aligned
    // by the maxmin_reorder function.

    double const topics_score = maxmin_reorder(extract_rows(topics),
                                               extract_rows(sklearn_topics),
                                               cosine_similarity);
    CHECK(topics_score > 0.9);

    double const docs_score = maxmin_reorder(extract_rows(xt::xtensor<double, 2>{xt::transpose(docs)}),
                                             extract_rows(xt::xtensor<double, 2>{xt::transpose(sklearn_docs)}),
                                             cosine_similarity);
    CHECK(docs_score > 0.9);
}

TEST_CASE("latent_dirichlet_allocation uses topic_word_preconditions")
{
    xt::xtensor<double, 2> const data = {
        {10, 8, 0, 1},
        { 7, 5, 1, 0},
        { 1, 0, 3, 0},
        { 0, 1, 5, 1},
        { 1, 0, 1, 2},
        { 1, 1, 0, 7},
    };

    latent_dirichlet_allocation::config config;
    config.topic_word_preconditions = {
        {1, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    };
    config.topic_count = config.topic_word_preconditions.shape()[0];
    config.topic_word_prior = 0.1;
    config.doc_topic_prior = 0.1;

    latent_dirichlet_allocation lda{config};
    lda.fit(data);

    xt::xtensor<double, 2> const topics = lda.topic_word_dirichlets();

    for (std::size_t topic_index = 0; topic_index < config.topic_count; ++topic_index) {
        double const similarity = cosine_similarity(
                xt::view(topics, topic_index),
                xt::view(config.topic_word_preconditions, topic_index));
        CHECK(similarity > 0.9);
    }
}
