#include <chrono>
#include <iostream>

#include <Eigen/Core>
#include <Eigen/CXX11/Tensor>
#include <Eigen/Dense>
#include <Eigen/SpecialFunctions>


int main()
{
    using Array = Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
    using Tensor3 = Eigen::Tensor<double, 3, Eigen::RowMajor>;

    long const doc_count = 30000;
    long const topic_count = 10;
    long const word_count = 10;

    Array beta(topic_count, word_count);
    Array lambda(doc_count, topic_count);
    Array beta_geoexp(topic_count, word_count);
    Array lambda_geoexp(doc_count, topic_count);
    Tensor3 phi(doc_count, word_count, topic_count);

    beta.setRandom();
    lambda.setRandom();

    auto const compute_geometric_expectation = [](auto const& dirichlets) {
        return (dirichlets.digamma().colwise()
            - dirichlets.rowwise().sum().digamma()).exp();
    };

    int const repeat_count = 100;
    auto const start = std::chrono::steady_clock::now();

    for (int repeat = 0; repeat < repeat_count; ++repeat) {
        beta_geoexp = compute_geometric_expectation(beta);
        lambda_geoexp = compute_geometric_expectation(lambda);

        for (int m = 0; m < doc_count; m++) {
            for (int i = 0; i < word_count; i++) {
                double sum = 0;
                for (int k = 0; k < topic_count; k++) {
                    auto weight = beta_geoexp(k, i) * lambda_geoexp(m, k);
                    phi(m, i, k) = weight;
                    sum += weight;
                }
                double norm = 1 / sum;
                for (int k = 0; k < topic_count; k++) {
                    phi(m, i, k) *= norm;
                }
            }
        }
    }

    auto const elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(
        std::chrono::steady_clock::now() - start).count();
    auto const cycle_time = elapsed_time / repeat_count;

    std::cout << cycle_time * 1e3 << " ms\n";
}
