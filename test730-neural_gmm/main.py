import keras
import keras.backend as K
import keras.layers as L
import numpy as np

from layers import Variable


def main():
    X = np.vstack(
        [
            np.random.normal(-2, 1, size=(8000, 1)),
            np.random.normal(1, 2, size=(8000, 1)),
        ]
    )
    neural_gmm(X, components=2)


def neural_gmm(X, components):
    data_dim = X.shape[1]

    inputs = L.Input((data_dim,))
    net = inputs

    net = L.Dense(100)(net)
    net = L.Activation("tanh")(net)

    net = L.Dense(components)(net)
    net = L.Activation("softmax")(net)
    weights = net

    mu = Variable((components, data_dim))(weights)
    logvar = Variable((components, data_dim))(weights)

    loss = logvar + K.exp(-logvar) * (K.expand_dims(inputs, axis=1) - mu) ** 2
    loss = K.sum(loss, axis=2)
    loss = K.sum(weights * loss, axis=1)
    loss = K.mean(loss, axis=0)

    model = keras.Model(inputs=inputs, outputs=[weights, mu, logvar])
    model.add_loss(loss)
    model.compile(optimizer="adam")

    train_options = {
        "epochs": 1000,
        "batch_size": 512,
        "validation_split": 0.1,
        "verbose": 2,
    }
    model.fit(X, **train_options)

    mus = K.eval(mu[0, :, 0])
    logvars = K.eval(logvar[0, :, 0])
    sigmas = np.exp(logvars / 2)

    for m, s in zip(mus, sigmas):
        print(f"{m:g}\t{s:g}")


if __name__ == "__main__":
    main()
