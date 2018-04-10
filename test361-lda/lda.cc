#include <array>
#include <iostream>
#include <cmath>
#include <cstddef>
#include <stdexcept>

#include <xtensor/xio.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xstrided_view.hpp>
#include <xtensor/xtensor.hpp>

#include "ext/copy.hpp"
#include "ext/xindex.hpp"
#include "ext/xmath.hpp"
#include "lda.hpp"


namespace
{
    constexpr double epsilon = 1e-6;

    // dirichlet_log_expect computes the expectation of the logarithm of
    // Dirichlet variables with given parameters on the last axis.
    template<typename E>
    auto dirichlet_log_expect(E&& params)
    {
        std::array<std::size_t, 1> const axis{params.dimension() - 1};
        return ext::digamma(ext::copy_expr(params))
                - ext::append_newaxis(ext::digamma(xt::sum(ext::copy_expr(params), ext::copy(axis))));
    }

    namespace ij
    {
        template<typename E>
        auto i(E&& vector)
        {
            return xt::view(vector, xt::all(), xt::newaxis());
        }
    }

    namespace ijk
    {
        // result(i, j, k) = matrix(i, j)
        template<typename E>
        auto ij(E&& matrix)
        {
            return xt::view(matrix, xt::all(), xt::all(), xt::newaxis());
        }

        // result(i, j, k) = matrix(j, i)
        template<typename E>
        auto ji(E&& matrix)
        {
            return xt::view(xt::transpose(matrix), xt::all(), xt::all(), xt::newaxis());
        }

        // result(i, j, k) = matrix(i, k)
        template<typename E>
        auto ik(E&& matrix)
        {
            return xt::view(matrix, xt::all(), xt::newaxis(), xt::all());
        }

        // result(i, j, k) = matrix(k, j)
        template<typename E>
        auto kj(E&& matrix)
        {
            return xt::view(xt::transpose(matrix), xt::newaxis(), xt::all(), xt::all());
        }

        // result(i, j, k) = tensor(j, i, k)
        template<typename E>
        auto jik(E&& tensor)
        {
            return xt::transpose(tensor);
        }
    }
}

latent_dirichlet_allocation::latent_dirichlet_allocation(config const& conf)
    : config_{conf}
{
}

void latent_dirichlet_allocation::train(xt::xtensor<double, 2> const& data)
{
    auto const data_shape = data.shape();
    auto const doc_count = data_shape[0];
    auto const word_count = data_shape[1];
    auto const topic_count = config_.topic_count;

    doc_topic_dirichlets_.resize({doc_count, topic_count});
    topic_word_dirichlets_.resize({topic_count, word_count});
    doc_word_topic_distr_.resize({doc_count, word_count, topic_count});

    xt::xtensor<double, 2> doc_topic_geoexp(doc_topic_dirichlets_.shape());
    xt::xtensor<double, 2> topic_word_geoexp(topic_word_dirichlets_.shape());
    xt::xtensor<double, 2> doc_word_topic_normalizer({doc_count, word_count});

    topic_word_dirichlets_ = config_.topic_word_preconditions;

    for (int outer_step = 0; outer_step < config_.outer_step_count; ++outer_step) {
        topic_word_geoexp = xt::exp(dirichlet_log_expect(topic_word_dirichlets_));

        doc_topic_dirichlets_ = 0.5 + xt::random::rand<double>(doc_topic_dirichlets_.shape());

        for (int inner_step = 0; inner_step < config_.inner_step_count; ++inner_step) {
            doc_topic_geoexp = xt::exp(dirichlet_log_expect(doc_topic_dirichlets_));

            doc_word_topic_distr_ = ijk::ik(doc_topic_geoexp) * ijk::kj(topic_word_geoexp);
            doc_word_topic_normalizer = 1 / (xt::sum(doc_word_topic_distr_, {2}) + epsilon);
            doc_word_topic_distr_ *= ijk::ij(doc_word_topic_normalizer);

            doc_topic_dirichlets_ = config_.doc_topic_prior
                + xt::sum(ijk::ij(data) * doc_word_topic_distr_, {1});
        }

        xt::transpose(topic_word_dirichlets_) = config_.topic_word_prior
            + xt::sum(ijk::ij(data) * doc_word_topic_distr_, {0});

        if (outer_step % 10 == 0) {
            std::clog << estimate_log_likelihood(data) << '\n';
        }
    }
}

double latent_dirichlet_allocation::estimate_log_likelihood(xt::xtensor<double, 2> const& data) const
{
    auto const topic_word_shape = topic_word_dirichlets_.shape();
    auto const topic_count = topic_word_shape[0];
    auto const word_count = topic_word_shape[1];

    if (data.shape()[1] != word_count) {
        throw std::logic_error("word count mismatch");
    }

    xt::xtensor<double, 2> const doc_topic_logexp = dirichlet_log_expect(doc_topic_dirichlets_);
    xt::xtensor<double, 2> const topic_word_logexp = dirichlet_log_expect(topic_word_dirichlets_);

    xt::xtensor<double, 1> const doc_topic_prior({topic_count}, config_.doc_topic_prior);
    xt::xtensor<double, 1> const topic_word_prior({word_count}, config_.topic_word_prior);

    auto const L_dt =
        xt::sum(xt::sum((config_.doc_topic_prior - doc_topic_dirichlets_) * doc_topic_logexp, {1})
                + ext::log_beta(doc_topic_dirichlets_)
                - ext::log_beta(doc_topic_prior));

    auto const L_tw =
        xt::sum(xt::sum((config_.topic_word_prior - topic_word_dirichlets_) * topic_word_logexp, {1})
                + ext::log_beta(topic_word_dirichlets_)
                - ext::log_beta(topic_word_prior));

    auto const doc_word_topic_X = doc_word_topic_distr_ * ijk::ij(data);
    auto const L_dwt =
        xt::sum(doc_word_topic_X * (ijk::ik(doc_topic_logexp)
                                    + ijk::kj(topic_word_logexp)
                                    - xt::log(doc_word_topic_distr_ + epsilon)));

    return (L_dt + L_tw + L_dwt)();
}

xt::xtensor<double, 2> latent_dirichlet_allocation::doc_topic_distribution() const
{
    return doc_topic_dirichlets_ / ij::i(xt::sum(doc_topic_dirichlets_, {1}));
}

xt::xtensor<double, 2> latent_dirichlet_allocation::topic_word_distribution() const
{
    return topic_word_dirichlets_ / ij::i(xt::sum(topic_word_dirichlets_, {1}));
}
