import numpy as np
import scipy.special as ss

def dirichlet_expect(distr):
    return ss.digamma(distr) - ss.digamma(np.sum(distr, axis=1))[:, np.newaxis]

class LatentDirichletAllocation(object):
    def __init__(self, n_topics, n_words):
        self._n_topics = n_topics
        self._n_words = n_words
        self._doc_topic_prior = np.ones(n_topics)
        self._topic_word_prior = np.ones(n_words)
        self._doc_topic_params = {'shape': 100, 'scale': 0.01}
        self._topic_word_params = {'shape': 100, 'scale': 0.01}
        self._max_em_iter = 100
        self._max_e_iter = 100

    def fit(self, X):
        """
        Train model for document-term matrix X.
        """
        topic_word_distr = np.random.gamma(size=(self._n_topics, self._n_words),
                                           **self._topic_word_params)

        for _ in range(self._max_em_iter):
            _, phi = self._e_step(X, topic_word_distr)
            topic_word_distr = self._m_step(X, phi)

        self._topic_word_distr = topic_word_distr

    def transform(self, X):
        """
        Transform document-term matrix X to document-topic distribution based
        on the trained model.
        """
        doc_topic_distr, _ = self._e_step(X, self._topic_word_distr)
        return doc_topic_distr / doc_topic_distr.sum(axis=1)[:, np.newaxis]

    def _e_step(self, X, topic_word_distr):
        """
        Estimate latent document-topic distribution and phi variable based on
        given data X and topic-word distribution.
        """
        n_docs, _ = X.shape
        doc_topic_distr = np.random.gamma(size=(n_docs, self._n_topics),
                                          **self._doc_topic_params)
        for _ in range(self._max_e_iter):
            exp_topic_word = np.exp(dirichlet_expect(topic_word_distr))
            exp_doc_topic = np.exp(dirichlet_expect(doc_topic_distr))
            phi = exp_doc_topic[:, np.newaxis, :] * exp_topic_word.T[np.newaxis, :, :]
            phi /= phi.sum(axis=2)[:, :, np.newaxis]

            doc_topic_distr = (X[:, :, np.newaxis] * phi).sum(axis=1)
            doc_topic_distr += self._doc_topic_prior[np.newaxis, :]

        return doc_topic_distr, phi

    def _m_step(self, X, phi):
        """
        Estimate latent topic-word distribution based on given X and phi.
        """
        topic_word_distr = (X[:, :, np.newaxis] * phi).sum(axis=0).T
        topic_word_distr += self._topic_word_prior[np.newaxis, :]
        return topic_word_distr

X = np.array([
    (10, 5, 0,  0, 0),
    ( 9, 4, 1,  0, 0),
    (15, 3, 0,  1, 0),
    ( 2, 1, 1,  5, 0),
    ( 1, 1, 2,  4, 0),
    ( 2, 0, 1,  8, 1),
    (10, 3, 4, 90, 5),
])

lda = LatentDirichletAllocation(n_topics=2, n_words=X.shape[1])
lda.fit(X)

np.set_printoptions(precision=2, suppress=True)

topic_word_distr = lda._topic_word_distr
topic_word_distr = topic_word_distr / topic_word_distr.sum(axis=1)[:, np.newaxis]
print('Topic-word distribution:')
print(topic_word_distr)
print('')

doc_topic_distr = lda.transform(X)
print('Document-topic distribution:')
print(doc_topic_distr)
