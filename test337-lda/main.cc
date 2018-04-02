#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xstrided_view.hpp>
#include <xtensor/xtensor.hpp>

#include "digamma.hpp"  // digamma
#include "index.hpp"    // ij, ijk


class latent_dirichlet_allocation
{
    static inline constexpr double epsilon = 1e-6;

  public:
    struct config
    {
        std::size_t topic_count = 2;
        double doc_topic_prior = 1;
        double topic_word_prior = 1;
        int m_step_count = 10;
        int e_step_count = 10;
        int evaluation_interval = 5;
    };

    explicit latent_dirichlet_allocation(config cfg)
        : topic_count_{cfg.topic_count}
        , doc_topic_prior_{cfg.doc_topic_prior}
        , topic_word_prior_{cfg.topic_word_prior}
        , m_step_count_{cfg.m_step_count}
        , e_step_count_{cfg.e_step_count}
        , evaluation_interval_{cfg.evaluation_interval}
    {
    }

    void fit(xt::xtensor<double, 2> const& data)
    {
        auto const data_shape = data.shape();
        auto const doc_count = data_shape[0];
        auto const word_count = data_shape[1];

        doc_topic_dirichlets_.resize({doc_count, topic_count_});
        topic_word_dirichlets_.resize({topic_count_, word_count});
        doc_word_topic_distr_.resize({doc_count, word_count, topic_count_});

        xt::xtensor<double, 2> doc_topic_geoexp(doc_topic_dirichlets_.shape());
        xt::xtensor<double, 2> topic_word_geoexp(topic_word_dirichlets_.shape());
        xt::xtensor<double, 2> doc_word_topic_norm({doc_count, word_count});

        topic_word_dirichlets_ = 0.5 + xt::random::rand<double>(topic_word_dirichlets_.shape());

        for (int m_step = 0; m_step < m_step_count_; ++m_step) {
            topic_word_geoexp = xt::exp(digamma(topic_word_dirichlets_)
                                - ij::i(digamma(xt::sum(topic_word_dirichlets_, {1}))));

            doc_topic_dirichlets_ = 0.5 + xt::random::rand<double>(doc_topic_dirichlets_.shape());

            for (int e_step = 0; e_step < e_step_count_; ++e_step) {
                doc_topic_geoexp = xt::exp(digamma(doc_topic_dirichlets_)
                                   - ij::i(digamma(xt::sum(doc_topic_dirichlets_, {1}))));

                doc_word_topic_distr_ = ijk::ik(doc_topic_geoexp) * ijk::kj(topic_word_geoexp);
                doc_word_topic_norm = 1 / (xt::sum(doc_word_topic_distr_, {2}) + epsilon);
                doc_word_topic_distr_ *= ijk::ij(doc_word_topic_norm);

                doc_topic_dirichlets_ = doc_topic_prior_ + xt::sum(ijk::ij(data) * doc_word_topic_distr_, {1});

                if (e_step % evaluation_interval_ == 0) {
                    std::clog << estimate_log_likelihood(data) << '\n';
                }
            }

            // Workaround: Direct application of xt::transpose() fails.
            xt::xtensor<double, 2> const transposed_topic_word_dirichlets =
                topic_word_prior_ + xt::sum(ijk::ij(data) * doc_word_topic_distr_, {0});
            topic_word_dirichlets_ = xt::transpose(transposed_topic_word_dirichlets);
        }
    }

    double estimate_log_likelihood(xt::xtensor<double, 2> const& data) const
    {
        auto const word_count = data.shape()[1];

        xt::xtensor<double, 2> dt = digamma(doc_topic_dirichlets_)
                                    - ij::i(digamma(xt::sum(doc_topic_dirichlets_, {1})));

        xt::xtensor<double, 2> tw = digamma(topic_word_dirichlets_)
                                    - ij::i(digamma(xt::sum(topic_word_dirichlets_, {1})));

        xt::xtensor<double, 3> dwt = ijk::ik(dt) + ijk::kj(tw)
                                     - xt::log(doc_word_topic_distr_ + epsilon);

        auto const& L_dt = xt::sum((doc_topic_prior_ - doc_topic_dirichlets_) * dt);
        auto const& L_tw = xt::sum((topic_word_prior_ - topic_word_dirichlets_) * tw);
        auto const& L_dwt = xt::sum(ijk::ij(data) * doc_word_topic_distr_ * dwt);

        xt::xtensor<double, 1> lbdt = xt::sum(xt::lgamma(doc_topic_dirichlets_), {1})
                                      - xt::lgamma(xt::sum(doc_topic_dirichlets_, {1}));

        xt::xtensor<double, 1> lbtw = xt::sum(xt::lgamma(topic_word_dirichlets_), {1})
                                      - xt::lgamma(xt::sum(topic_word_dirichlets_, {1}));

        auto const& dt_prior = topic_count_ * std::lgamma(doc_topic_prior_)
                               - std::lgamma(topic_count_ * doc_topic_prior_);

        auto const& tw_prior = word_count * std::lgamma(topic_word_prior_)
                               - std::lgamma(word_count * topic_word_prior_);

        auto const& L_lbdt = xt::sum(lbdt + dt_prior);
        auto const& L_lbtw = xt::sum(lbtw + tw_prior);

        return (L_dt + L_lbdt + L_tw + L_lbtw + L_dwt)();
    }

    xt::xtensor<double, 2> doc_topic_distribution() const
    {
        return doc_topic_dirichlets_ / ij::i(xt::sum(doc_topic_dirichlets_, {1}));
    }

    xt::xtensor<double, 2> topic_word_distribution() const
    {
        return topic_word_dirichlets_ / ij::i(xt::sum(topic_word_dirichlets_, {1}));
    }

  private:
    std::size_t topic_count_;
    double doc_topic_prior_;
    double topic_word_prior_;

    int m_step_count_;
    int e_step_count_;
    int evaluation_interval_;

    xt::xtensor<double, 2> doc_topic_dirichlets_;
    xt::xtensor<double, 2> topic_word_dirichlets_;
    xt::xtensor<double, 3> doc_word_topic_distr_;
};


int main()
{
    latent_dirichlet_allocation::config config;
    config.topic_count = 5;
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
