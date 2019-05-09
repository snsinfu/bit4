# Layer does not track child layers. Model does.
# compute_output_shape needs to be implemented.

import numpy as np
import keras
import keras.backend as K
import keras.layers as L
import tensorflow as tf


data_dim = 10
hidden_dim = 20
latent_dim = 5


def main():
    ae = make_autoencoder()
    ae.compile(optimizer="adam")
    ae.summary()

    X = np.random.uniform(-2, 2, size=(20000, data_dim))
    ae.fit(X, batch_size=32, epochs=10)


class Encoder(keras.Model):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.dense1 = L.Dense(hidden_dim, activation="elu")
        self.dense2 = L.Dense(latent_dim, activation="sigmoid")

    def compute_output_shape(self, input_shape):
        return input_shape[0], self.dense2.units

    def call(self, x):
        net = x
        net = self.dense1(net)
        net = self.dense2(net)
        return net


class Decoder(keras.Model):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.dense = L.Dense(data_dim)

    def compute_output_shape(self, input_shape):
        return input_shape[0], self.dense.units

    def call(self, x):
        return self.dense(x)


def make_autoencoder():
    encoder = Encoder()
    decoder = Decoder()

    x = L.Input((data_dim,), name="x")
    z = encoder(x)
    y = decoder(z)
    autoencoder = keras.Model(x, y, name="autoencoder")

    loss = (y - x) ** 2
    loss = K.sum(loss, axis=1)
    loss = K.mean(loss, axis=0)
    autoencoder.add_loss(loss)

    return autoencoder


if __name__ == "__main__":
    main()
