import signal

import numpy as np
import scipy.special as ss


EPSILON = 1e-6


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    X = np.tile([(1, 0, 0), (0, 1, 0), (0, 0, 1)], reps=(10000, 1))
    X = X * 5 + np.random.poisson(5, size=X.shape)

    topic_word_preconditions = [
        (1, 0, 0),
        (0, 1, 0),
        (0, 0, 1),
        (1, 1, 0),
        (0, 1, 1),
    ]

    lda = LatentDirichletAllocation(topic_step_count=100,
                                    doc_step_count=100,
                                    topic_word_preconditions=topic_word_preconditions)
    Y = lda.fit_transform(X)
    Y = Y / np.sum(Y, axis=1)[:, np.newaxis]

    np.set_printoptions(precision=2, suppress=True)
    twd = lda.topic_word_dirichlets
    twd = twd / np.sum(twd, axis=1)[:, np.newaxis]

    print(twd)
    print(Y)


class LatentDirichletAllocation:
    def __init__(self,
                 topic_word_preconditions=None,
                 doc_topic_prior=1.0,
                 topic_word_prior=1.0,
                 topic_step_count=10,
                 doc_step_count=10):

        if topic_word_preconditions is None:
            self._topic_word_preconditions = None
        else:
            self._topic_word_preconditions = np.asarray(topic_word_preconditions)

        self._doc_topic_prior = doc_topic_prior
        self._topic_word_prior = topic_word_prior
        self._topic_step_count = topic_step_count
        self._doc_step_count = doc_step_count

    def fit(self, X):
        X = np.asarray(X)

        topic_count, word_count = self._topic_word_preconditions.shape
        doc_count, data_word_count = X.shape

        if data_word_count != word_count:
            raise ValueError('shape mismatch')

        topic_word_dirichlets = self._topic_word_preconditions
        for topic_step in range(self._topic_step_count):
            topic_word_geoexp = dirichlet_geoexp(topic_word_dirichlets + EPSILON)

            doc_topic_dirichlets = np.random.uniform(size=(doc_count, topic_count))
            doc_topic_dirichlets += self._doc_topic_prior

            for doc_step in range(self._doc_step_count):
                doc_topic_geoexp = dirichlet_geoexp(doc_topic_dirichlets + EPSILON)

                doc_word_topic_distr = (doc_topic_geoexp[:, np.newaxis, :]
                                        * topic_word_geoexp.T[np.newaxis, :, :])
                doc_word_topic_distr *= 1 / (EPSILON + np.sum(doc_word_topic_distr, axis=2))[:, :, np.newaxis]

                doc_word_topic_X = doc_word_topic_distr * X[:, :, np.newaxis]

                doc_topic_dirichlets = np.sum(doc_word_topic_X, axis=1)
                doc_topic_dirichlets += self._doc_topic_prior

            topic_word_dirichlets = np.sum(doc_word_topic_X, axis=0).T
            topic_word_dirichlets += self._topic_word_prior

        self._doc_topic_dirichlets = doc_topic_dirichlets
        self._topic_word_dirichlets = topic_word_dirichlets
        self._doc_word_topic_distr = doc_word_topic_distr

        return self

    def fit_transform(self, X):
        return self.fit(X)._doc_topic_dirichlets

    def score(self, X):
        X = np.asarray(X)

        topic_count, word_count = self._topic_word_dirichlets.shape
        doc_count, data_word_count = X.shape
        if data_word_count != word_count:
            raise ValueError('shape mismatch')

        doc_topic_logexp = dirichlet_logexp(self._doc_topic_dirichlets + EPSILON)
        topic_word_logexp = dirichlet_logexp(self._topic_word_dirichlets + EPSILON)

        L_dt = np.sum((self._doc_topic_prior - self._doc_topic_dirichlets) * doc_topic_logexp
                      + log_beta(self._doc_topic_dirichlets)
                      - log_beta([self._doc_topic_prior] * topic_count)[0])

        L_tw = np.sum((self._topic_word_prior - self._topic_word_dirichlets) * topic_word_logexp
                      + log_beta(self._topic_word_dirichlets)
                      - log_beta([self._topic_word_prior] * word_count)[0])

        L_dwt = np.sum(X[:, :, np.newaxis] * self._doc_word_topic_distr
                       * (doc_topic_logexp[:, np.newaxis, :] + topic_word_logexp.T[np.newaxis, :, :])
                       - np.log(self._doc_word_topic_distr))

        return L_dt + L_tw + L_dwt

    @property
    def topic_word_dirichlets(self):
        return self._topic_word_dirichlets


def log_beta(X):
    return np.sum(ss.gammaln(X), axis=1) - ss.gammaln(np.sum(X, axis=1))


def dirichlet_logexp(X):
    return ss.digamma(X) - ss.digamma(np.sum(X, axis=1))[:, np.newaxis]


def dirichlet_geoexp(X):
    return np.exp(dirichlet_logexp(X))


if __name__ == '__main__':
    main()
