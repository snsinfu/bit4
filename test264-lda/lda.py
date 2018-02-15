import numpy as np
import scipy.special as ss
from sklearn.base import BaseEstimator, TransformerMixin


DEFAULT_DOC_TOPIC_INIT = {'shape': 100, 'scale': 0.01}
DEFAULT_TOPIC_WORD_INIT = {'shape': 100, 'scale': 0.01}


class LatentDirichletAllocation(BaseEstimator, TransformerMixin):
    def __init__(self,
                 n_components,
                 doc_topic_init=DEFAULT_DOC_TOPIC_INIT,
                 doc_topic_prior=1.0,
                 topic_word_init=DEFAULT_TOPIC_WORD_INIT,
                 topic_word_prior=1.0,
                 max_iter=10,
                 seed=None):
        self.n_components = n_components
        self.doc_topic_init = doc_topic_init
        self.doc_topic_prior = doc_topic_prior
        self.topic_word_init = topic_word_init
        self.topic_word_prior = topic_word_prior
        self.max_iter = max_iter

        if seed is None:
            self._random = np.random.random.__self__
        else:
            if isinstance(seed, np.random.RandomState):
                self._random = seed
            else:
                self._random = np.random.RandomState(seed)

    def fit(self, X):
        _, n_words = X.shape

        topic_word_distr = initial_distr(self.topic_word_init,
                                         size=(self.n_components, n_words),
                                         random=self._random)
        for _ in range(self.max_iter):
            _, phi = self._e_step(X, topic_word_distr)
            topic_word_distr = self._m_step(X, phi)

        self.topic_word_distr_ = topic_word_distr

        return self

    def transform(self, X):
        doc_topic_distr, _ = self._e_step(X, self.topic_word_distr_)
        return doc_topic_distr / doc_topic_distr.sum(axis=1)[:, np.newaxis]

    def _e_step(self, X, topic_word_distr):
        n_docs, _ = X.shape

        doc_topic_distr = initial_distr(self.doc_topic_init,
                                        size=(n_docs, self.n_components),
                                        random=self._random)
        for _ in range(self.max_iter):
            exp_topic_word = np.exp(dirichlet_expect(topic_word_distr))
            exp_doc_topic = np.exp(dirichlet_expect(doc_topic_distr))

            phi = exp_doc_topic[:, np.newaxis, :] * exp_topic_word.T[np.newaxis, :, :]
            phi /= phi.sum(axis=2)[:, :, np.newaxis]

            doc_topic_distr = (X[:, :, np.newaxis] * phi).sum(axis=1)
            doc_topic_distr += self.doc_topic_prior

        return doc_topic_distr, phi

    def _m_step(self, X, phi):
        topic_word_distr = (X[:, :, np.newaxis] * phi).sum(axis=0).T
        topic_word_distr += self.topic_word_prior
        return topic_word_distr


def dirichlet_expect(distr):
    return ss.digamma(distr) - ss.digamma(np.sum(distr, axis=1))[:, np.newaxis]


def initial_distr(init, size, random):
    if isinstance(init, dict):
        return initial_distr_gamma(init, size, random)

    if isinstance(init, np.ndarray):
        return initial_distr_fixed(init, size)

    return initial_distr_uniform(init, size)


def initial_distr_gamma(params, size, random):
    return random.gamma(size=size, **params)


def initial_distr_fixed(init, size):
    assert init.shape == size # FIXME: error check
    return init


def initial_distr_uniform(init, size):
    distr = np.empty(size)
    distr[:] = init
    return distr
