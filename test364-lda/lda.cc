#include <cassert>
#include <cstddef>
#include <stdexcept>

#include <xtensor/xmath.hpp>
#include <xtensor/xnoalias.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xshape.hpp>
#include <xtensor/xstrided_view.hpp>
#include <xtensor/xtensor.hpp>

#include "ijk.hpp"
#include "lda.hpp"
#include "math.hpp"


namespace
{
    // epsilon value used to prevent zero division and zero logarithm.
    constexpr double epsilon = 1e-6;

    // Computes the expectation of the logarithm of Dirichlet variables with
    // given parameters on the last axis.
    template<typename E>
    auto dirichlet_log_expect(E&& params)
    {
        return digamma(+params)
                - xt::view(digamma(xt::sum(+params, {params.dimension() - 1})),
                           xt::all(), xt::newaxis());
    }

    // Computes the geometric expectation of Dirichlet variables with given
    // parameters on the last axis.
    template<typename E>
    auto dirichlet_geometric_expect(E&& params)
    {
        return xt::exp(dirichlet_log_expect(std::forward<E>(params)));
    }

    // Validates LDA configuration.
    void validate(latent_dirichlet_allocation::config const& conf)
    {
        if (!(conf.convergence_threshold > 0)) {
            throw std::domain_error("convergence_threshold must be a positive number");
        }

        if (!(conf.doc_topic_prior > 0)) {
            throw std::domain_error("doc_topic_prior must be a positive number");
        }

        if (!(conf.topic_word_prior > 0)) {
            throw std::domain_error("topic_word_prior must be a positive number");
        }

        if (!(conf.outer_iter_count > 0)) {
            throw std::domain_error("outer_iter_count must be a positive integer");
        }

        if (!(conf.inner_iter_count > 0)) {
            throw std::domain_error("inner_iter_count must be a positive integer");
        }

        if (conf.topic_word_preconditions.size()
            && conf.topic_word_preconditions.shape()[0] != conf.topic_count) {
            throw std::domain_error("topic_word_preconditions shape inconsistent with topic_count");
        }
    }
}

latent_dirichlet_allocation::latent_dirichlet_allocation(config const& conf)
    : config_{conf}
{
    validate(config_);
}

void latent_dirichlet_allocation::fit(xt::xtensor<double, 2> const& data)
{
    auto const data_shape = data.shape();
    auto const doc_count = data_shape[0];
    auto const word_count = data_shape[1];
    auto const topic_count = config_.topic_count;

    init_topic_word_dirichlets(topic_count, word_count);

    xt::xtensor<double, 3> doc_word_topic_distr{xt::static_shape<std::size_t, 3>{doc_count, word_count, topic_count}};
    xt::xtensor<double, 2> prev_topic_word_dirichlets = topic_word_dirichlets_;

    for (int iter = 0; iter < config_.outer_iter_count; ++iter) {
        transform(data, doc_word_topic_distr);

        xt::transpose(topic_word_dirichlets_) = config_.topic_word_prior
                                              + xt::sum(ijk::ij(data) * doc_word_topic_distr, {0});

        double const max_delta = xt::amax(xt::abs(topic_word_dirichlets_ - prev_topic_word_dirichlets))();
        if (max_delta <= config_.convergence_threshold) {
            break;
        }
        prev_topic_word_dirichlets = topic_word_dirichlets_;
    }
}

xt::xtensor<double, 2> latent_dirichlet_allocation::transform(
        xt::xtensor<double, 2> const& data) const
{
    auto const topic_count = config_.topic_count;
    auto const doc_count = data.shape()[0];
    auto const word_count = data.shape()[1];

    xt::xtensor<double, 3> doc_word_topic_distr{xt::static_shape<std::size_t, 3>{doc_count, word_count, topic_count}};

    return transform(data, doc_word_topic_distr);
}

xt::xtensor<double, 2> latent_dirichlet_allocation::transform(
        xt::xtensor<double, 2> const& data,
        xt::xtensor<double, 3>& doc_word_topic_distr) const
{
    auto const doc_count = data.shape()[0];
    auto const word_count = data.shape()[1];
    auto const topic_count = config_.topic_count;

    xt::xtensor<double, 2> doc_topic_dirichlets = config_.doc_topic_prior
                                                + xt::random::rand<double>(xt::static_shape<std::size_t, 2>{doc_count, topic_count});
    xt::xtensor<double, 2> prev_doc_topic_dirichlets{doc_topic_dirichlets.shape()};

    xt::xtensor<double, 2> doc_topic_geoexp{xt::static_shape<std::size_t, 2>{doc_count, topic_count}};
    xt::xtensor<double, 2> doc_word_topic_norm{xt::static_shape<std::size_t, 2>{doc_count, word_count}};
    xt::xtensor<double, 2> const topic_word_geoexp = dirichlet_geometric_expect(topic_word_dirichlets_);

    for (int inner_iter = 0; inner_iter < config_.inner_iter_count; ++inner_iter) {
        xt::noalias(doc_topic_geoexp) = dirichlet_geometric_expect(doc_topic_dirichlets);

        xt::noalias(doc_word_topic_distr) = ijk::ik(doc_topic_geoexp) * ijk::kj(topic_word_geoexp);
        xt::noalias(doc_word_topic_norm) = 1 / (xt::sum(doc_word_topic_distr, {2}) + epsilon);
        xt::noalias(doc_word_topic_distr) *= ijk::ij(doc_word_topic_norm);

        xt::noalias(doc_topic_dirichlets) = config_.doc_topic_prior
                                          + xt::sum(ijk::ij(data) * doc_word_topic_distr, {1});

        double const max_delta = xt::amax(xt::abs(doc_topic_dirichlets - prev_doc_topic_dirichlets))();
        if (max_delta <= config_.convergence_threshold) {
            break;
        }
        xt::noalias(prev_doc_topic_dirichlets) = doc_topic_dirichlets;
    }

    return doc_topic_dirichlets;
}

double latent_dirichlet_allocation::score(
        xt::xtensor<double, 2> const& data) const
{
    auto const doc_count = data.shape()[0];
    auto const word_count = data.shape()[1];
    auto const topic_count = config_.topic_count;

    xt::xtensor<double, 3> doc_word_topic_distr{xt::static_shape<std::size_t, 3>{doc_count, word_count, topic_count}};
    xt::xtensor<double, 2> doc_topic_dirichlets = transform(data, doc_word_topic_distr);

    return estimate_log_likelihood(data, doc_topic_dirichlets, doc_word_topic_distr);
}

xt::xtensor<double, 2> latent_dirichlet_allocation::topic_word_dirichlets() const
{
    return topic_word_dirichlets_;
}

void latent_dirichlet_allocation::init_topic_word_dirichlets(
        std::size_t topic_count, std::size_t word_count)
{
    xt::xtensor<double, 2> const& preconditions = config_.topic_word_preconditions;

    if (preconditions.size() == 0) {
        randomize_topic_word_dirichlets(topic_count, word_count);
        return;
    }

    if (config_.topic_word_preconditions.shape()[1] != word_count) {
        throw std::domain_error(
            "training word count is inconsistent with the shape of "
            "topic_word_preconditions");
    }

    topic_word_dirichlets_ = config_.topic_word_prior + config_.topic_word_preconditions;
}

void latent_dirichlet_allocation::randomize_topic_word_dirichlets(
        std::size_t topic_count, std::size_t word_count)
{
    topic_word_dirichlets_ = config_.topic_word_prior
                           + xt::random::rand<double>(xt::static_shape<std::size_t, 2>{topic_count, word_count});
}

double latent_dirichlet_allocation::estimate_log_likelihood(
        xt::xtensor<double, 2> const& data,
        xt::xtensor<double, 2> const& doc_topic_dirichlets,
        xt::xtensor<double, 3> const& doc_word_topic_distr) const
{
    auto const topic_word_shape = topic_word_dirichlets_.shape();
    auto const topic_count = topic_word_shape[0];
    auto const word_count = topic_word_shape[1];

    if (data.shape()[1] != word_count) {
        throw std::logic_error("word count mismatch");
    }

    xt::xtensor<double, 2> const doc_topic_logexp = dirichlet_log_expect(doc_topic_dirichlets);
    xt::xtensor<double, 2> const topic_word_logexp = dirichlet_log_expect(topic_word_dirichlets_);

    xt::xtensor<double, 1> const doc_topic_prior(xt::static_shape<std::size_t, 1>{topic_count}, config_.doc_topic_prior);
    xt::xtensor<double, 1> const topic_word_prior(xt::static_shape<std::size_t, 1>{word_count}, config_.topic_word_prior);

    auto const L_dt = xt::sum(xt::sum((config_.doc_topic_prior - doc_topic_dirichlets) * doc_topic_logexp, {1})
                              + log_beta(doc_topic_dirichlets)
                              - log_beta(doc_topic_prior));

    auto const L_tw = xt::sum(xt::sum((config_.topic_word_prior - topic_word_dirichlets_) * topic_word_logexp, {1})
                              + log_beta(topic_word_dirichlets_)
                              - log_beta(topic_word_prior));

    auto const doc_word_topic_X = doc_word_topic_distr * ijk::ij(data);
    auto const L_dwt = xt::sum(doc_word_topic_X * (ijk::ik(doc_topic_logexp)
                               + ijk::kj(topic_word_logexp)
                               - xt::log(doc_word_topic_distr + epsilon)));

    return (L_dt + L_tw + L_dwt)();
}

