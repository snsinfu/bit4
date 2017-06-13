#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Print matrix to stdout in tabular form.
void print_matrix(
    int rows, int cols, const double matrix[rows][cols]
);

// Fit LDA to given word frequency data and return latent distributions.
void latent_dirichlet_allocation(
    int n_docs, int n_words, int n_topics,
    const double word_freq[n_docs][n_words],
    double doc_topic_distr[n_docs][n_topics],
    double topic_word_distr[n_topics][n_words]
);

int main(void) {
    const int n_docs = 7;
    const int n_words = 5;
    const int n_topics = 2;

    const double word_freq[n_docs][n_words] = {
        {10, 5, 0,  0, 0},
        { 9, 4, 1,  0, 0},
        {15, 3, 0,  1, 0},
        { 2, 1, 1,  5, 0},
        { 1, 1, 2,  4, 0},
        { 2, 0, 1,  8, 1},
        {10, 3, 4, 90, 5},
    };

    double doc_topic_distr[n_docs][n_topics];
    double topic_word_distr[n_topics][n_words];

    latent_dirichlet_allocation(
        n_docs, n_words, n_topics,
        word_freq, doc_topic_distr, topic_word_distr
    );

    puts("Topic-word distribution:");
    print_matrix(n_topics, n_words, topic_word_distr);
    puts("");
    puts("Document-topic distribution:");
    print_matrix(n_docs, n_topics, doc_topic_distr);

    return 0;
}

void print_matrix(int rows, int cols, const double matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%.3g%c", matrix[i][j], "\t\n"[j + 1 == cols]);
        }
    }
}

// Compute approximate digamma function.
static double digamma(
    double x
);

// Randomize matrix elements with numbers in [lo,hi].
static void randomize_matrix(
    int rows, int cols, double matrix[rows][cols], double lo, double hi
);

// Compute E[log X] for Dirichlet variable X.
static void exp_dirichlet_expect(
    int n, const double distr[n], double result[n]
);

// Normalize probability distribution.
static void normalize_distribution(
    int n, double distr[n]
);

void latent_dirichlet_allocation(
    int n_docs, int n_words, int n_topics,
    const double word_freq[n_docs][n_words],
    double doc_topic_distr[n_docs][n_topics],
    double topic_word_distr[n_topics][n_words]
) {
    // Heuristic priors from sklearn
    const double doc_topic_prior = 1. / n_topics;
    const double topic_word_prior = 1. / n_topics;

    // Inference parameters
    const int max_em_iter = 100;
    const int max_e_iter = 100;

    // Start EM iterations
    randomize_matrix(n_topics, n_words, topic_word_distr, 0.9, 1.1);

    for (int em_iter = 0; em_iter < max_em_iter; em_iter++) {

        double phi[n_docs][n_words][n_topics];

        // E step: Estimate document-topic distribution and phi
        randomize_matrix(n_docs, n_topics, doc_topic_distr, 0.9, 1.1);

        for (int e_iter = 0; e_iter < max_e_iter; e_iter++) {

            // phi_mik ~ exp( psi(beta_ki) - psi(sum_i' beta_ki')
            //                + psi(lambda_mk) - psi(sum_k' lambda_mk') )

            double exp_topic_word[n_topics][n_words];
            double exp_doc_topic[n_docs][n_topics];

            for (int k = 0; k < n_topics; k++) {
                exp_dirichlet_expect(n_words, topic_word_distr[k], exp_topic_word[k]);
            }
            for (int m = 0; m < n_docs; m++) {
                exp_dirichlet_expect(n_topics, doc_topic_distr[m], exp_doc_topic[m]);
            }

            for (int m = 0; m < n_docs; m++) {
                for (int i = 0; i < n_words; i++) {
                    for (int k = 0; k < n_topics; k++) {
                        phi[m][i][k] = exp_topic_word[k][i] * exp_doc_topic[m][k];
                    }
                    normalize_distribution(n_topics, phi[m][i]);
                }
            }

            // Estimate document-topic distribution
            // lambda_mk = alpha + sum_i X_mi phi_mik

            for (int m = 0; m < n_docs; m++) {
                for (int k = 0; k < n_topics; k++) {
                    double sum = doc_topic_prior;
                    for (int i = 0; i < n_words; i++) {
                        sum += word_freq[m][i] * phi[m][i][k];
                    }
                    doc_topic_distr[m][k] = sum;
                }
            }
        }

        // M step: Estimate topic-word distribution
        // beta_ki = eta + sum_m X_mi phi_mik

        for (int k = 0; k < n_topics; k++) {
            for (int i = 0; i < n_words; i++) {
                double sum = topic_word_prior;
                for (int m = 0; m < n_docs; m++) {
                    sum += word_freq[m][i] * phi[m][i][k];
                }
                topic_word_distr[k][i] = sum;
            }
        }
    }

    for (int m = 0; m < n_docs; m++) {
        normalize_distribution(n_topics, doc_topic_distr[m]);
    }
    for (int k = 0; k < n_topics; k++) {
        normalize_distribution(n_words, topic_word_distr[k]);
    }
}

double digamma(double x) {
    if (x < 1e-6) {
        const double EULER = 0.5772156649015328606;
        return -EULER - 1 / x;
    }

    double result = 0;

    // psi(x + 1) = psi(x) + 1/x
    while (x < 6) {
        result -= 1 / x;
        x += 1;
    }

    // psi(x) = log(x) - 1/(2x) - 1/(12x^2) + 1/(120x^4) - 1/(252x^6) + ...
    double r = 1 / x;
    result += log(x) - 5 * r;
    r *= r;
    result -= r * (1./12 - r * (1./120 - r * (1./252)));

    return result;
}

void randomize_matrix(int rows, int cols, double matrix[rows][cols], double lo, double hi) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = lo + (hi - lo) * rand() / RAND_MAX;
        }
    }
}

void exp_dirichlet_expect(int n, const double distr[n], double result[n]) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += distr[i];
    }

    const double digamma_sum = digamma(sum);
    for (int i = 0; i < n; i++) {
        result[i] = exp(digamma(distr[i]) - digamma_sum);
    }
}

void normalize_distribution(int n, double distr[n]) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += distr[i];
    }

    const double norm = 1 / sum;
    for (int i = 0; i < n; i++) {
        distr[i] *= norm;
    }
}
