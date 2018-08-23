import numpy as np

from gensim.models.ldamodel import LdaModel


# Gensim LDA. The single-core version (not parallelized LdaMulticore) supports
# fitting of assymmetric priors (alpha and eta). Though it requires input data
# to be in the form of some sparse matrix. Still it's great!


def format_array(arr):
    rank = len(arr.shape)
    if rank > 1:
        newlines = "\n" * (rank - 1)
        return newlines.join(format_array(sub) for sub in arr)
    return "\t".join("{:.2g}".format(x) for x in arr)


def matrix_to_corpus(mat):
    return [list(enumerate(x)) for x in mat]


data = np.array([
    (10, 5, 0,  0, 0),
    ( 9, 4, 1,  0, 0),
    (15, 3, 0,  1, 0),
    ( 2, 1, 1,  5, 0),
    ( 1, 1, 2,  4, 0),
    ( 2, 0, 1,  8, 1),
    (10, 3, 4, 90, 5),
])
labels = ("A", "B", "C", "D", "E")


train_data = np.vstack([
    ((data + np.random.uniform(0.0, 2.0, size=data.shape[1])[np.newaxis, :])
           * np.random.uniform(0.2, 5.0, size=data.shape[0])[:, np.newaxis])
    for _ in range(100)
])
test_data = data

np.random.shuffle(train_data)


train_corpus = matrix_to_corpus(train_data)
test_corpus = matrix_to_corpus(test_data)
id2word = list(enumerate(labels))

random_state = np.random.RandomState(seed=0)
hyper_parameters = {"num_topics": 3, "alpha": "auto", "eta": "auto"}
training_parameters = {"passes": 1, "chunksize": 1000, "iterations": 1000}
api_parameters = {"minimum_probability": 0}

kwargs = dict(**hyper_parameters, **training_parameters, **api_parameters)
lda = LdaModel(train_corpus, random_state=random_state, **kwargs)


# (gensim 3.5.0) log_perplexity actually returns variational likelihood bound
# [1][2], which is loosely an approximation of -log(perplexity) but the error
# depends on how variational EM algorithm went. So in theory this should not be
# used to find out the optimal number of topics. Also some people say perplexity
# is not a good measure of topic quality [3]. c_v topic coherence might be a
# better measure [4].
#
# Anyway, here I assume that the likelihood bound is a fairly tight one and
# actually approximates the true perplexity.
#
# [1]: https://radimrehurek.com/gensim/models/ldamodel.html#gensim.models.ldamodel.LdaModel.log_perplexity
# [2]: https://stats.stackexchange.com/a/324243
# [3]: https://groups.google.com/forum/#!topic/gensim/TpuYRxhyIOc
# [4]: https://www.kdnuggets.com/2016/07/americas-next-topic-model.html

perp = np.exp(-lda.log_perplexity(test_corpus))
loglik = -len(test_corpus) * np.log(perp)
doc_topic = np.array(list(lda.get_document_topics(test_corpus)))[:, :, 1]

print("Alpha:")
print(format_array(lda.alpha))

print("\nEta:")
print(format_array(lda.eta))

print("\nTopic-word distribution:")
print(format_array(lda.get_topics()))

print("\nDocument-topic distribution:")
print(format_array(doc_topic))

print("\nPerplexity (lower the better):")
print("{:.6g}".format(perp))

print("\nLog likelihood:")
print("{:.6g}".format(loglik))
