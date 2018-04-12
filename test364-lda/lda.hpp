#ifndef INCLUDED_LDA_HPP
#define INCLUDED_LDA_HPP

#include <cstddef>

#include <xtensor/xtensor.hpp>


class latent_dirichlet_allocation
{
  public:

    // Hyperparameters for latent_dirichlet_allocation.
    struct config
    {
        // The number of topics.
        std::size_t topic_count = 2;

        // Optional preconditioning for topic-word dirichlet parameters.
        //
        // If set, the topic-word dirichlet parameters are initialized to
        // `topic_word_prior + topic_word_preconditions` at the beginning of
        // each training.
        xt::xtensor<double, 2> topic_word_preconditions = {{}};

        // Symmetric prior for document-topic dirichlet parameters.
        double doc_topic_prior = 1;

        // Symmetric prior for topic-word dirichlet parameters.
        double topic_word_prior = 1;

        // The maximum number of iterations for fitting topic-word dirichlet
        // parameters. The worst-case time complexity of fit operation is
        // proportional to this count.
        int outer_iter_count = 100;

        // The maximum number of iterations for fitting document-topic
        // dirichlet parameters. The worst-case time complexity of fit and
        // transform operations are proportional to this count.
        int inner_iter_count = 100;

        // Fitting iteration is stopped earlily if the maximum absolute change
        // of dirichlet parameters is less than this threshold.
        double convergence_threshold = 1e-4;
    };

    // Creates an untrained model with given configuration.
    explicit latent_dirichlet_allocation(config const& conf);

    // Trains the model with given data.
    void fit(xt::xtensor<double, 2> const& data);

    // Computes the document-topic dirichlet parameters for given data using a
    // trained model.
    xt::xtensor<double, 2> transform(
            xt::xtensor<double, 2> const& data) const;

    // Estimates log-likelihood of given data for a trained model.
    double score(xt::xtensor<double, 2> const& data) const;

    // Returns the topic-word dirichlet parameters of a trained model.
    xt::xtensor<double, 2> topic_word_dirichlets() const;

  private:
    // Computes the document-topic dirichlet parameters and document-word-topic
    // distribution for a trained (or on-being-trained) model.
    xt::xtensor<double, 2> transform(
            xt::xtensor<double, 2> const& data,
            xt::xtensor<double, 3>& doc_word_topic_distr) const;

    // Initializes the internal topic-word dirichlet parameters based on the
    // configuration given on construction.
    void init_topic_word_dirichlets(
            std::size_t topic_count, std::size_t word_count);

    // Initializes the internal topic-word dirichlet parameters to random
    // values.
    void randomize_topic_word_dirichlets(
            std::size_t topic_count, std::size_t word_count);

    // Estimates log-likelihood of data with given parameters.
    double estimate_log_likelihood(
            xt::xtensor<double, 2> const& data,
            xt::xtensor<double, 2> const& doc_topic_dirichlets,
            xt::xtensor<double, 3> const& doc_word_topic_distr) const;

  private:
    config config_;
    xt::xtensor<double, 2> topic_word_dirichlets_ = {{}};
};

#endif
