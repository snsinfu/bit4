import numpy as np
from scipy import special

def dirichlet_expect(distr):
    return special.digamma(distr) - special.digamma(np.sum(distr, axis=1))[:, np.newaxis]

class LatentDirichletAllocation(object):
    def __init__(self, n_topics, doc_topic_prior=None, topic_word_prior=None):
        self.n_topics_ = n_topics
        self.doc_topic_prior_ = doc_topic_prior or 1.0
        self.topic_word_prior_ = topic_word_prior or 1.0
        self.max_e_iter_ = 100
        self.max_m_iter_ = 100

    def set_topic_word_distr(self, topic_word_distr):
        self.topic_word_distr_ = np.asarray(topic_word_distr)

    def fit(self, X, y=None):
        if y is None:
            self.fit_unsupervised(X)
        else:
            self.fit_supervised(X, y)

    def fit_transform(self, X):
        self.fit(X)
        return self.doc_topic_distr_

    def fit_unsupervised(self, X):
        X = np.asarray(X)

        n_topics = self.n_topics_
        n_docs, n_words = X.shape

        doc_topic_prior = np.ones(n_topics) * self.doc_topic_prior_
        topic_word_prior = np.ones(n_words) * self.topic_word_prior_

        topic_word_distr = np.ones((n_topics, n_words))
        topic_word_distr *= topic_word_prior[np.newaxis, :]

        for _ in range(self.max_m_iter_):
            doc_topic_distr = np.random.gamma(1, 1.0, size=(n_docs, n_topics))
            doc_topic_distr += doc_topic_prior[np.newaxis, :]

            exp_topic_word = np.exp(dirichlet_expect(topic_word_distr))

            for _ in range(self.max_e_iter_):
                exp_doc_topic = np.exp(dirichlet_expect(doc_topic_distr))

                phi = exp_doc_topic[:, np.newaxis, :] * exp_topic_word.T[np.newaxis, :, :]
                phi /= np.sum(phi, axis=2)[:, :, np.newaxis]
                phi *= X[:, :, np.newaxis]

                doc_topic_distr = phi.sum(axis=1)
                doc_topic_distr += doc_topic_prior[np.newaxis, :]

            topic_word_distr = phi.sum(axis=0).T
            topic_word_distr += topic_word_prior[np.newaxis, :]

        self.topic_word_distr_ = topic_word_distr
        self.doc_topic_distr_ = doc_topic_distr

    def fit_supervised(self, X, y):
        X = np.asarray(X)
        y = np.asarray(y)

        n_topics = self.n_topics_
        n_docs, n_words = X.shape

        assert y.shape[0] == n_docs
        assert y.shape[1] == n_topics

        doc_topic_distr = y

        topic_word_prior = np.ones(n_words) * self.topic_word_prior_

        topic_word_distr = np.ones((n_topics, n_words))
        topic_word_distr *= topic_word_prior[np.newaxis, :]

        exp_doc_topic = np.exp(dirichlet_expect(doc_topic_distr))

        for _ in range(self.max_m_iter_):
            exp_topic_word = np.exp(dirichlet_expect(topic_word_distr))

            phi = exp_doc_topic[:, np.newaxis, :] * exp_topic_word.T[np.newaxis, :, :]
            phi /= np.sum(phi, axis=2)[:, :, np.newaxis]

            topic_word_distr = (X[:, :, np.newaxis] * phi).sum(axis=0).T
            topic_word_distr += topic_word_prior[np.newaxis, :]

        self.topic_word_distr_ = topic_word_distr
        self.doc_topic_distr_ = doc_topic_distr

def normalize(X, axis=-1):
    return X / np.sum(X, axis=axis, keepdims=True)

# Same documents in different languages (or different word choices)

X_lang1 = np.array([
    (10, 5, 0,  0, 0, 2),
    ( 9, 4, 1,  0, 0, 1),
    (15, 3, 0,  1, 0, 2),
    ( 2, 1, 1,  5, 0, 1),
    ( 1, 1, 2,  4, 0, 1),
    ( 2, 0, 1,  8, 1, 1),
    (10, 3, 4, 90, 5, 5),
])

X_lang2 = np.array([
    ( 3,  9, 1, 5),
    ( 2,  5, 0, 2),
    ( 3, 13, 1, 2),
    ( 7,  0, 2, 0),
    ( 8,  1, 3, 1),
    ( 5,  0, 8, 1),
    (12,  5, 9, 5),
])

np.set_printoptions(precision=2, suppress=True)

# Train the model in the first language
lda = LatentDirichletAllocation(n_topics=2)
lda.fit(X_lang1)

print('Topic-word distribution in the first language:')
print(normalize(lda.topic_word_distr_ - 1))

# Transfer knowledge
lda.fit(X_lang2, lda.doc_topic_distr_)

print('')
print('Topic-word distribution in the second language:')
print(normalize(lda.topic_word_distr_ - 1))
