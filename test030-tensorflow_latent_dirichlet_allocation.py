import numpy as np
import tensorflow as tf

FLOAT_TYPE = tf.float64

# Data
X = np.array([
    (10, 5, 0,  0, 0),
    ( 9, 4, 1,  0, 0),
    (15, 3, 0,  1, 0),
    ( 2, 1, 1,  5, 0),
    ( 1, 1, 2,  4, 0),
    ( 2, 0, 1,  8, 1),
    (10, 3, 4, 90, 5),
])
n_docs, n_words = X.shape

# Hyper-parameters
n_topics = 2
doc_topic_prior = 1.0
topic_word_prior = 1.0
np.random.seed(1234)

# Data as tensor
doc_word_matrix = tf.constant(X, shape=X.shape, dtype=FLOAT_TYPE)

# Latent variables
doc_topic_distr = tf.Variable(tf.random_gamma((n_docs, n_topics),
                                              dtype=FLOAT_TYPE,
                                              alpha=100, beta=0.01,
                                              seed=np.random.randint(1 << 32)),
                              name='doc_topic_distr')

topic_word_distr = tf.Variable(tf.random_gamma((n_topics, n_words),
                                               dtype=FLOAT_TYPE,
                                               alpha=100, beta=0.01,
                                               seed=np.random.randint(1 << 32)),
                               name='topic_word_distr')

# E-step formula
def expect_log_dirichlet(distr):
    sumdistr = tf.reduce_sum(distr, axis=-1, keep_dims=True)
    return tf.digamma(distr) - tf.digamma(sumdistr)

exp_doc_topic = expect_log_dirichlet(doc_topic_distr)
exp_topic_word = expect_log_dirichlet(topic_word_distr)

updater = tf.exp(tf.expand_dims(exp_doc_topic, 2) +
                 tf.expand_dims(exp_topic_word, 0))
updater /= tf.reduce_sum(updater, axis=1, keep_dims=True)
updater *= tf.expand_dims(doc_word_matrix, 1)

e_iter_op = tf.assign(doc_topic_distr,
                      doc_topic_prior + tf.reduce_sum(updater, axis=2))

_, e_step_op = tf.while_loop(lambda i, _: i < 100,
                             lambda i, _: (i + 1, e_iter_op),
                             [tf.constant(0), doc_topic_distr])

# M-step formula
m_step_op = tf.assign(topic_word_distr,
                      topic_word_prior + tf.reduce_sum(updater, axis=0))

# Train
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    sess.run(topic_word_distr.initializer)
    for em in range(100):
        sess.run(doc_topic_distr.initializer)
        sess.run(e_step_op)
        sess.run(m_step_op)

    em_lambda = sess.run(topic_word_distr)
    em_gamma = sess.run(doc_topic_distr)

# Inspect
print('Topic-word distribution:')
print(em_lambda / em_lambda.sum(axis=1)[:, np.newaxis])
print('')
print('Document-topic distribution:')
print(em_gamma / em_gamma.sum(axis=1)[:, np.newaxis])
