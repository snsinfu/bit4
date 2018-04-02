#include <array>
#include <cstddef>
#include <iostream>

#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xrandom.hpp>
#include <xtensor/xstrided_view.hpp>
#include <xtensor/xtensor.hpp>

#include "digamma.hpp"  // digamma
#include "index.hpp"    // ij, ijk


int main()
{
    std::size_t const doc_count = 30000;
    std::size_t const topic_count = 10;
    std::size_t const word_count = 10;

    int const e_step_count = 10;
    int const m_step_count = 10;
    double const phi_epsilon = 1e-6;

    double const eta = 1.0;
    double const alpha = 1.0;

    xt::xtensor<double, 2> X({doc_count, word_count});

    X = xt::random::randn<double>(X.shape());

    xt::xtensor<double, 2> beta({topic_count, word_count});
    xt::xtensor<double, 2> lambda({doc_count, topic_count});
    xt::xtensor<double, 3> phi({doc_count, word_count, topic_count});

    xt::xtensor<double, 2> beta_geoexp(beta.shape());
    xt::xtensor<double, 2> lambda_geoexp(lambda.shape());
    xt::xtensor<double, 2> phi_norm({phi.shape()[0], phi.shape()[1]});

    auto const start = std::chrono::steady_clock::now();

    for (int m_step = 0; m_step < m_step_count; ++m_step) {
        lambda = xt::random::randn<double>(lambda.shape());

        for (int e_step = 0; e_step < e_step_count; ++e_step) {
            beta = xt::random::randn<double>(beta.shape());

            beta_geoexp = xt::exp(digamma(beta) - ij::i(digamma(xt::sum(beta, {1}))));
            lambda_geoexp = xt::exp(digamma(lambda) - ij::i(digamma(xt::sum(lambda, {1}))));

            phi = ijk::kj(beta_geoexp) * ijk::ik(lambda_geoexp);
            phi_norm = 1 / (xt::sum(phi, {2}) + phi_epsilon);
            phi *= ijk::ij(phi_norm);

            lambda = alpha + xt::sum(ijk::ij(X) * phi, {1});
        }

        beta = eta + xt::sum(ijk::ij(X) * phi, {0});
    }

    auto const elapsed_time
        = std::chrono::duration_cast<std::chrono::duration<double>>(
            std::chrono::steady_clock::now() - start).count();

    auto const cycle_time = elapsed_time / (e_step_count * m_step_count);

    std::cout << cycle_time * 1e3 << " ms\n";
}
