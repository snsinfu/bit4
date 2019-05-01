import os

os.environ["KMP_DUPLICATE_LIB_OK"] = "True"

import numpy as np
import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


latent_dim = 2
data_dim = 15


def main():
    encoder_module = Encoder()
    decoder_module = Decoder()
    x_in = L.Input((data_dim,))
    logit = encoder_module(x_in)
    z_out = GumbelSoftmax()(logit)
    alpha_out = decoder_module(z_out)

    # Dirichlet log-likelihood
    loglik = K.sum((alpha_out - 1) * safe_log(x_in), axis=1)
    loglik += tf.math.lgamma(K.sum(alpha_out, axis=1))
    loglik -= K.sum(tf.math.lgamma(alpha_out), axis=1)

    # Entropy of the latent softmax distributionn
    theta = K.softmax(logit)
    entropy = -theta * safe_log(theta)
    entropy = K.sum(entropy, axis=1)

    # Entropy-regularized NLL
    #
    # Mild regularization that keeps entropy from decreasinng seems effective
    # for learning decoder weights.
    entropy_weight = -0.3
    loss = -loglik + entropy_weight * entropy
    loss = K.mean(loss, axis=0)

    autoencoder = keras.Model(x_in, alpha_out)
    autoencoder.add_loss(loss)
    autoencoder.add_metric(loglik, name="loglik", aggregation="mean")
    autoencoder.add_metric(entropy, name="entropy", aggregation="mean")
    autoencoder.compile(optimizer=keras.optimizers.Adam(0.02))

    encoder = keras.Sequential([L.InputLayer((data_dim,)), encoder_module, L.Softmax()])
    decoder = keras.Sequential([L.InputLayer((latent_dim,)), decoder_module])

    # Fitting
    alpha1 = np.array([0.1, 0.2, 0.3, 0.4, 0.5] * (data_dim // 5))
    alpha2 = np.array([1.0, 2.0, 3.0, 4.0, 5.0] * (data_dim // 5))
    X = np.concatenate(
        [
            np.random.dirichlet(alpha1, size=50000),
            np.random.dirichlet(alpha2, size=50000),
        ]
    )
    np.random.shuffle(X)

    autoencoder.fit(X, batch_size=500, validation_split=0.1, epochs=10, verbose=2)

    # Test
    emission_matrix = decoder.predict(np.eye(latent_dim))
    print(emission_matrix)


def safe_log(x):
    x = K.maximum(x, K.epsilon())
    return K.log(x)


class Encoder(L.Layer):
    def __init__(self):
        super().__init__()
        self._dense1 = L.Dense(5, kernel_regularizer=keras.regularizers.l2(0.01))
        self._dense2 = L.Dense(20, activation="elu")
        self._norm2 = LayerNormalization()
        self._drop2 = L.Dropout(0.3)
        self._classifier = L.Dense(latent_dim)

    def compute_output_shape(self, input_shape):
        return input_shape[0], latent_dim

    def call(self, x):
        y = x
        y = self._dense1(y)
        y = self._dense2(y)
        y = self._norm2(y)
        y = self._drop2(y)
        y = self._classifier(y)
        return y


class Decoder(L.Layer):
    def __init__(self):
        super().__init__()
        self._dense = L.Dense(data_dim, activation="softplus")

    def compute_output_shape(self, input_shape):
        return input_shape[0], data_dim

    def call(self, z):
        return self._dense(z)


class GumbelSoftmax(L.Layer):
    def __init__(self, temperature=1, **kwargs):
        super().__init__(**kwargs)
        self.initial_temperature = temperature

    def build(self, input_shape):
        self.temperature = K.variable(self.initial_temperature)
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, inputs, training=None):
        shape = K.shape(inputs)
        u = K.random_uniform(shape)
        u = K.clip(u, K.epsilon(), 1 - K.epsilon())
        logit = inputs - K.log(-K.log(u))
        softmax = K.softmax(logit / self.temperature)
        hardmax = K.one_hot(K.argmax(logit), shape[-1])
        hardmax = K.reshape(K.cast(hardmax, K.dtype(softmax)), shape)
        output = K.in_train_phase(softmax, hardmax, training=training)
        return output


class LayerNormalization(L.Layer):
    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        self.var_epsilon = K.epsilon() ** 2
        super().build(input_shape)

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.var_epsilon)
        output = self.gamma * output + self.beta
        return output


if __name__ == "__main__":
    main()
