# TF Keras is ~20% slower on this particular task.

import numpy as np
import tensorflow.keras as keras
import tensorflow.keras.backend as K
import tensorflow.keras.layers as L
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


class Encoder(L.Layer):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.dense1 = L.Dense(hidden_dim, activation="elu")
        self.dense2 = L.Dense(latent_dim, activation="sigmoid")

    def call(self, x):
        net = x
        net = self.dense1(net)
        net = self.dense2(net)
        return net


class Decoder(L.Layer):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.dense = L.Dense(data_dim)

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
