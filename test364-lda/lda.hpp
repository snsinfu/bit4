#ifndef INCLUDED_LDA_HPP
#define INCLUDED_LDA_HPP

#include <cstddef>

#include <xtensor/xtensor.hpp>


class latent_dirichlet_allocation
{
  public:

    struct config
    {
        std::size_t topic_count = 2;
        xt::xtensor<double, 2> topic_word_preconditions = {{}};

        double doc_topic_prior = 1;
        double topic_word_prior = 1;

        int outer_iter_count = 100;
        int inner_iter_count = 100;
        double convergence_threshold = 1e-4;
    };

    //
    explicit latent_dirichlet_allocation(config const& conf);

    //
    void fit(xt::xtensor<double, 2> const& data);

    //
    xt::xtensor<double, 2> transform(
            xt::xtensor<double, 2> const& data) const;

    //
    xt::xtensor<double, 2> transform(
            xt::xtensor<double, 2> const& data,
            xt::xtensor<double, 3>& doc_word_topic_distr) const;

    //
    double score(xt::xtensor<double, 2> const& data) const;

    //
    xt::xtensor<double, 2> topic_word_distribution() const;

  private:
    void init_topic_word_dirichlets(
            std::size_t topic_count, std::size_t word_count);

    void randomize_topic_word_dirichlets(
            std::size_t topic_count, std::size_t word_count);

    double estimate_log_likelihood(
            xt::xtensor<double, 2> const& data,
            xt::xtensor<double, 2> const& doc_topic_dirichlets,
            xt::xtensor<double, 3> const& doc_word_topic_distr) const;

  private:
    config config_;
    xt::xtensor<double, 2> topic_word_dirichlets_;
};

#endif
