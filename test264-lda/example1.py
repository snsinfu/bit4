import numpy as np

from lda import LatentDirichletAllocation


X = np.array([
    (10, 5, 0,  0, 0),
    ( 9, 4, 1,  0, 0),
    (15, 3, 0,  1, 0),
    ( 2, 1, 1,  5, 0),
    ( 1, 1, 2,  4, 0),
    ( 2, 0, 1,  8, 1),
    (10, 3, 4, 90, 5),
])

topic_word_init = np.array([
    [1.0, 0.0, 0.0, 0.0, 0.0],
    [0.0, 0.0, 0.0, 1.0, 0.0],
]) + 1.0

lda = LatentDirichletAllocation(n_components=2,
                                topic_word_init=topic_word_init)
lda.fit(X)

np.set_printoptions(precision=2, suppress=True)

topic_word_distr = lda.topic_word_distr_
topic_word_distr = topic_word_distr / topic_word_distr.sum(axis=1)[:, np.newaxis]
print('Topic-word distribution:')
print(topic_word_distr)
print('')

doc_topic_distr = lda.transform(X)
print('Document-topic distribution:')
print(doc_topic_distr)
