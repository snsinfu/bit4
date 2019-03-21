"""
Gamma-Poisson (negative binomial) fitting.
"""

import numpy as np
import tensorflow as tf


def main():
    true_alpha = 4.7
    true_theta = 0.6

    p = 1 / (1 + true_theta)
    data = np.random.negative_binomial(true_alpha, p, size=10000)
    alpha, theta = fit_gamma_poisson(data)

    print("param\ttruth\test.")
    print(f"alpha\t{true_alpha:g}\t{alpha:g}")
    print(f"theta\t{true_theta:g}\t{theta:g}")


def fit_gamma_poisson(data, epochs=100, lr=0.001, momentum=0.99, batch_size=1000):
    """
    Fit Gamma-Poisson model to data.
    """
    x = tf.placeholder(dtype=tf.float32)

    log_alpha = tf.Variable(0, dtype=tf.float32)
    log_theta = tf.Variable(0, dtype=tf.float32)
    alpha = tf.math.exp(log_alpha)
    theta = tf.math.exp(log_theta)

    # Negative log-likelihood
    loss = (
        tf.math.lgamma(alpha)
        + tf.math.lgamma(1 + x)
        - tf.math.lgamma(alpha + 1 + x)
        + tf.math.log(alpha + x)
        + (alpha + x) * tf.math.log(1 + theta)
        - x * log_theta
    )
    loss = tf.math.reduce_mean(loss)

    # Training
    optimizer = tf.train.MomentumOptimizer(
        learning_rate=lr, momentum=momentum, use_nesterov=True
    )
    optimizer_op = optimizer.minimize(loss=loss)

    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())

        # Minibatch updates
        data = np.copy(data)
        for _ in range(epochs):
            np.random.shuffle(data)
            for beg in range(0, len(data), batch_size):
                end = min(beg + batch_size, len(data))
                sess.run(optimizer_op, feed_dict={x: data[beg:end]})

        alpha_val, theta_val = sess.run([alpha, theta])

    return alpha_val, theta_val


if __name__ == "__main__":
    main()
