import keras
import keras.layers as L
import numpy as np

from sklearn.model_selection import train_test_split

from layers import LayerNormalization
from modules import AttentionEncoder


def main():
    np.random.seed(0)

    sample_count = 10000
    sequence_length = 100

    X, y = make_adding_problem(sample_count, sequence_length)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    train_options = {
        "batch_size": 32,
        "epochs": 10,
        "validation_split": 0.1,
        "verbose": 1,
    }

    # Simple MLP (~2546 parameters) -- Test error: 0.33
    simple_mlp = make_simple_mlp(sequence_length)
    simple_mlp.compile(loss="mae", optimizer="adam")
    simple_mlp.fit(X_train, y_train, **train_options)
    y_pred = simple_mlp.predict(X_test, batch_size=train_options["batch_size"])
    test_error = np.mean(np.abs(y_pred - y_test))
    print("Simple MLP:", test_error)

    # TCN (~782 parameters) -- Test error: 0.062
    tcn = make_tcn(sequence_length)
    tcn.compile(loss="mae", optimizer="adam")
    tcn.fit(X_train, y_train, **train_options)
    y_pred = tcn.predict(X_test, batch_size=train_options["batch_size"])
    test_error = np.mean(np.abs(y_pred - y_test))
    print("TCN:", test_error)

    # Attention (726 parameters -- Test error: 0.047
    attention = make_attention(sequence_length)
    attention.compile(loss="mae", optimizer="adam")
    attention.fit(X_train, y_train, **train_options)
    y_pred = attention.predict(X_test, batch_size=train_options["batch_size"])
    test_error = np.mean(np.abs(y_pred - y_test))
    print("Attention:", test_error)


def make_adding_problem(sample_count, sequence_length):
    numbers = np.random.uniform(0, 1, size=(sample_count, sequence_length))
    choices = np.zeros((sample_count, sequence_length))
    sums = np.zeros(sample_count)

    seeds = np.random.uniform(0, 1, size=(sample_count, sequence_length))
    selectors = np.argsort(seeds, axis=1)[:, :2]
    for i, selector in enumerate(selectors):
        choices[i, selector] = 1
        sums[i] = numbers[i, selector].sum()

    X = np.stack([numbers, choices], axis=-1)
    y = sums.reshape(-1, 1)

    return X, y


def make_simple_mlp(sequence_length):
    inputs = L.Input((sequence_length, 2))

    dense_options = {
        "kernel_initializer": "he_normal",
    }

    net = inputs
    net = L.Dense(5, **dense_options)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    net = L.Flatten()(net)
    net = L.Dense(5, **dense_options)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("relu")(net)

    net = L.Dense(1, **dense_options)(net)
    outputs = net

    return keras.Model(inputs, outputs)


def make_tcn(sequence_length):
    inputs = L.Input((sequence_length, 2))

    dense_options = {
        "kernel_initializer": "he_normal",
    }

    net = inputs
    net = L.Dense(5, **dense_options)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    conv_options = {
        "padding": "same",
        "kernel_initializer": "he_normal",
    }

    for i in range(10):
        dilation = 2 ** i
        if dilation > sequence_length // 2:
            break

        res = net
        net = L.Conv1D(5, kernel_size=3, **conv_options)(net)
        net = L.BatchNormalization()(net)
        net = L.Activation("relu")(net)
        net = L.add([res, net])

    net = L.Dense(2, **dense_options)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("relu")(net)

    net = L.Flatten()(net)
    net = L.Dense(1, **dense_options)(net)
    outputs = net

    return keras.Model(inputs, outputs)


def make_attention(sequence_length):
    inputs = L.Input((sequence_length, 2))

    dense_options = {
        "kernel_initializer": "he_normal"
    }

    net = inputs
    net = L.Dense(5, **dense_options)(net)
    net = LayerNormalization()(net)
    net = L.Activation("tanh")(net)

    net = AttentionEncoder(keys=5, values=5, inner_dim=5)(net)

    net = L.Flatten()(net)
    net = L.Dense(1, **dense_options)(net)
    outputs = net

    return keras.Model(inputs, outputs)


if __name__ == "__main__":
    main()
