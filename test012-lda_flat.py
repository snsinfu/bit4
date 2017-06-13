import numpy as np
import scipy.special as ss

def main():
    X = np.array([
        (10, 5, 0,  0, 0),
        ( 9, 4, 1,  0, 0),
        (15, 3, 0,  1, 0),
        ( 2, 1, 1,  5, 0),
        ( 1, 1, 2,  4, 0),
        ( 2, 0, 1,  8, 1),
        (10, 3, 4, 90, 5),
    ])
    doc_topic_distr, topic_word_distr = latent_dirichlet_allocation(X, 2)

    np.set_printoptions(precision=2, suppress=True)
    print('Topic-word distribution:')
    print(topic_word_distr)
    print('')
    print('Document-topic distribution:')
    print(doc_topic_distr)

def latent_dirichlet_allocation(X, n_topics):
    n_docs, n_words = X.shape

    # These priors are taken from the sklearn implementation
    doc_topic_prior = np.ones(n_topics) / n_topics
    topic_word_prior = np.ones(n_words) / n_topics

    # These initial values are taken from the sklearn implementation
    doc_topic_init = np.random.gamma(100, 0.01, size=(n_docs, n_topics))
    topic_word_init = np.random.gamma(100, 0.01, size=(n_topics, n_words))

    max_em_iter = 100
    max_e_iter = 100

    topic_word_distr = topic_word_init

    for _ in range(max_em_iter):
        # [E-step] Estimate document-topic distribution and latent phi
        doc_topic_distr = doc_topic_init

        for _ in range(max_e_iter):
            exp_topic_word = np.exp(dirichlet_expect(topic_word_distr))
            exp_doc_topic = np.exp(dirichlet_expect(doc_topic_distr))

            # phi[m, i, k] ~ exp_topic_word[m, k] * exp_doc_topic[k, i]
            phi = exp_doc_topic[:, np.newaxis, :] * exp_topic_word.T[np.newaxis, :, :]
            phi /= phi.sum(axis=2)[:, :, np.newaxis]

            doc_topic_distr = (X[:, :, np.newaxis] * phi).sum(axis=1)
            doc_topic_distr += doc_topic_prior[np.newaxis, :]

        # [M-step] Estimate topic-word distribution
        topic_word_distr = (X[:, :, np.newaxis] * phi).sum(axis=0).T
        topic_word_distr += topic_word_prior[np.newaxis, :]

    doc_topic_distr /= doc_topic_distr.sum(axis=1)[:, np.newaxis]
    topic_word_distr /= topic_word_distr.sum(axis=1)[:, np.newaxis]

    return doc_topic_distr, topic_word_distr

def dirichlet_expect(distr):
    return ss.digamma(distr) - ss.digamma(np.sum(distr, axis=1))[:, np.newaxis]

if __name__ == '__main__':
    main()
