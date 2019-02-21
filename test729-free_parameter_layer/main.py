import keras
import keras.backend as K
import keras.layers as L
import numpy as np

from layers import FreeParameters


def main():
    # Toy dataset
    input_dim = 5
    output_dim = 2

    X = np.random.normal(0, 0.1, size=(10000, input_dim))
    y = np.transpose([np.sign(X[:, 0]) * np.cos(X[:, 1]), np.sin(X[:, 1]) * X[:, 0]])
    y += np.random.gamma(2, 0.1, size=y.shape)

    # Model
    x_inputs = L.Input((input_dim,))
    y_inputs = L.Input((output_dim,))

    net = x_inputs
    net = L.Dense(50, activation="tanh")(net)
    net = L.Dense(50, activation="tanh")(net)
    net = L.Dense(2, activation="tanh")(net)
    v = FreeParameters((output_dim,))(x_inputs)

    loss = (net - y_inputs)**2 * K.exp(-v) + v + 4
    loss = K.sum(loss, axis=1)
    loss = K.mean(loss, axis=0)

    model = keras.Model(inputs=[x_inputs, y_inputs], outputs=[net, v])
    model.add_loss(loss)
    model.compile(optimizer="adam")

    # Training
    training_options = {
        "epochs": 200,
        "batch_size": 250,
        "validation_split": 0.1,
        "verbose": 2,
    }
    history = model.fit([X, y], **training_options)

    #
    _, v_values = model.predict([X, y])
    print(v_values[0])


if __name__ == "__main__":
    main()
