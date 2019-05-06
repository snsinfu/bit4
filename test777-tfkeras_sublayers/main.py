import os

os.environ["KMP_DUPLICATE_LIB_OK"] = "True"

import numpy as np
import tensorflow as tf
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


def main():
    use_subclassing = True
    data_dim = 5
    latent_dim = 2

    if use_subclassing:
        autoencoder = Autoencoder(data_dim, latent_dim)
    else:
        autoencoder = make_autoencoder(data_dim, latent_dim)
    autoencoder.summary()
    autoencoder.compile(optimizer=keras.optimizers.Adam(0.01))

    # Fitting
    alpha1 = 1 + np.arange(data_dim)
    alpha2 = alpha1[::-1]
    train_X = np.concatenate(
        [
            np.random.dirichlet(alpha1, size=10000),
            np.random.dirichlet(alpha2, size=10000),
        ]
    )
    np.random.shuffle(train_X)

    autoencoder.fit(train_X, batch_size=100, validation_split=0.1, epochs=10, verbose=2)

    # Save and load
    autoencoder.save_weights("weights.h5")
    autoencoder.load_weights("weights.h5")


def make_autoencoder(data_dim, latent_dim):
    temperature = 1.0
    entropy_weight = 1.0

    encoder_module = EncoderModule(latent_dim, name="encoder")
    decoder_module = DecoderModule(data_dim, name="decoder")

    x_in = L.Input((data_dim,))
    latent_logits = encoder_module(x_in)
    latent_code = GumbelSoftmax(temperature)(latent_logits)
    alpha_out = decoder_module(latent_code)

    # Dirichlet log-likelihood
    loglik = K.sum((alpha_out - 1) * safe_log(x_in), axis=1)
    loglik += tf.math.lgamma(K.sum(alpha_out, axis=1))
    loglik -= K.sum(tf.math.lgamma(alpha_out), axis=1)

    # Entropy of latent softmax
    theta = K.softmax(latent_logits)
    entropy = -theta * safe_log(theta)
    entropy = K.sum(entropy, axis=1)

    # Beta-VAE loss with uniform prior
    loss = -loglik - entropy_weight * entropy
    loss = K.mean(loss, axis=0)

    autoencoder = keras.Model(x_in, alpha_out)
    autoencoder.add_loss(loss)
    autoencoder.add_metric(loglik, name="loglik", aggregation="mean")
    autoencoder.add_metric(entropy, name="entropy", aggregation="mean")

    return autoencoder


class Autoencoder(keras.Model):
    def __init__(self, data_dim, latent_dim, **kwargs):
        super().__init__(**kwargs)
        self.encoder_module = EncoderModule(latent_dim)
        self.gumbel_softmax = GumbelSoftmax()
        self.decoder_module = DecoderModule(data_dim)

        # XXX: Is this a hack or a proper use of the Keras API? The documentation [1]
        # does not mention but without __call__() or at least build() the model fails
        # to display summary() or running fit().
        # [1]: https://www.tensorflow.org/alpha/guide/keras/custom_layers_and_models
        self.__call__(L.Input((data_dim,)))

    def call(self, x):
        logits = self.encoder_module(x)
        code = self.gumbel_softmax(logits)
        alpha = self.decoder_module(code)
        self._setup_metrics(x, logits, alpha)
        return alpha

    def _setup_metrics(self, x, logits, alpha):
        # Dirichlet log likelihood
        loglik = K.sum((alpha - 1) * safe_log(x), axis=1)
        loglik += tf.math.lgamma(K.sum(alpha, axis=1))
        loglik -= K.sum(tf.math.lgamma(alpha), axis=1)

        # Entropy of latent softmax
        theta = K.softmax(logits)
        entropy = -theta * safe_log(theta)
        entropy = K.sum(entropy, axis=1)

        # Beta-VAE loss with uniform prior
        recons_loss = K.mean(-loglik, axis=0)
        kl_loss = K.mean(-entropy, axis=0)

        self.add_loss(recons_loss)
        self.add_loss(kl_loss)
        self.add_metric(loglik, name="loglik", aggregation="mean")
        self.add_metric(entropy, name="entropy", aggregation="mean")


class EncoderModule(L.Layer):
    def __init__(self, latent_dim, **kwargs):
        super().__init__(**kwargs)
        self._dense1 = L.Dense(5, activation="sigmoid", name="dense1")
        self._norm1 = LayerNormalization(name="norm1")
        self._dense2 = L.Dense(20, activation="elu", name="dense2")
        self._norm2 = LayerNormalization(name="norm2")
        self._classifier = L.Dense(latent_dim, name="classifier")

    def call(self, x):
        y = safe_log(x)
        y = self._dense1(y)
        y = self._norm1(y)
        y = self._dense2(y)
        y = self._norm2(y)
        y = self._classifier(y)
        return y


class DecoderModule(L.Layer):
    def __init__(self, output_dim, **kwargs):
        super().__init__(**kwargs)
        self._dense = L.Dense(output_dim, activation="softplus", name="output")

    def call(self, x):
        return self._dense(x)


class GumbelSoftmax(L.Layer):
    def __init__(self, temperature=1, **kwargs):
        super().__init__(**kwargs)
        self.initial_temperature = temperature

    def build(self, input_shape):
        self.temperature = K.variable(self.initial_temperature)
        super().build(input_shape)

    def call(self, inputs, training=None):
        shape = K.shape(inputs)
        u = K.random_uniform(shape)
        logit = inputs - safe_log(-safe_log(u))
        return K.softmax(logit / self.temperature)


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


def safe_log(x):
    return K.log(K.maximum(x, K.epsilon()))


if __name__ == "__main__":
    main()
