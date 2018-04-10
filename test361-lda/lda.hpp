#ifndef INCLUDED_LDA_HPP
#define INCLUDED_LDA_HPP

#include <cmath>
#include <cstddef>

#include <xtensor/xmath.hpp>
#include <xtensor/xtensor.hpp>


class latent_dirichlet_allocation
{
  public:
    struct config
    {
        std::size_t topic_count = 2;
        double doc_topic_prior = 1;
        double topic_word_prior = 1;
        int outer_step_count = 10;
        int inner_step_count = 10;
        xt::xtensor<double, 2> topic_word_preconditions;
    };

    explicit latent_dirichlet_allocation(config const& conf);

    void train(xt::xtensor<double, 2> const& data);
    double estimate_log_likelihood(xt::xtensor<double, 2> const& data) const;

    xt::xtensor<double, 2> doc_topic_distribution() const;
    xt::xtensor<double, 2> topic_word_distribution() const;

  private:
    config config_;
    xt::xtensor<double, 2> doc_topic_dirichlets_;
    xt::xtensor<double, 2> topic_word_dirichlets_;
    xt::xtensor<double, 3> doc_word_topic_distr_;
};

#endif
