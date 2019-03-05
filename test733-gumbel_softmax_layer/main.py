import keras
import keras.backend as K
import keras.layers as L
import numpy as np

from layers import GumbelSoftmax


data_dim = 3
hidden_dim = 20
latent_dim = 3
gumbel_temperature = 1.0


def main():
    inputs = L.Input((data_dim,))

    net = inputs
    net = L.Dense(hidden_dim)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    net = L.Dense(latent_dim)(net)
    net = GumbelSoftmax(gumbel_temperature)(net)
    encoder = keras.Model(inputs, net)

    net = L.Dense(hidden_dim)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    net = L.Dense(data_dim * 16)(net)
    net = L.Reshape((data_dim, 16))(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("softmax")(net)

    net = L.Dense(1)(net)
    net = L.Reshape((data_dim,))(net)
    outputs = net

    loss = (outputs - inputs) ** 2
    loss = K.sum(loss, axis=1)
    loss = K.mean(loss, axis=0)

    autoencoder = keras.Model(inputs=inputs, outputs=outputs)
    autoencoder.add_loss(loss)
    autoencoder.compile(optimizer="adam")

    print(autoencoder.summary())

    # Gaussian mixture
    mu1 = [0, 1, 1]
    mu2 = [2, 0, 0]
    mu3 = [1, 2, 3]

    X = np.vstack([
        np.random.normal(mu1, 0.5, size=(10000, 3)),
        np.random.normal(mu2, 0.5, size=(10000, 3)),
        np.random.normal(mu3, 0.5, size=(10000, 3)),
    ])

    train_options = {
        "epochs": 30,
        "batch_size": 128,
        "validation_split": 0.1,
        "verbose": 1,
    }
    autoencoder.fit(X, **train_options)

    # The latent code should become almost one-hot.
    test_X = np.vstack([
        np.random.normal(mu1, 0.5, size=(3, 3)),
        np.random.normal(mu2, 0.5, size=(3, 3)),
        np.random.normal(mu3, 0.5, size=(3, 3)),
    ])
    test_z = encoder.predict(test_X)

    for p1, p2, p3 in test_z:
        print(f"{p1:.4f}\t{p2:.4f}\t{p3:.4f}")


if __name__ == "__main__":
    main()
