import logging
import os

# Suppress INFO and WARNING from Tensorflow
logging.getLogger("tensorflow").setLevel(logging.ERROR)
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"

# Suppress verbose log from Intel OMP
os.environ["KMP_AFFINITY"] = "scatter,none"


import keras
import keras.layers as L
import numpy as np


def main():
    experiment_single_output()
    experiment_single_output_with_metric()
    experiment_multiple_outputs()


def experiment_single_output():
    model = keras.Sequential()
    model.add(L.Dense(10, activation="elu", input_dim=2))
    model.add(L.Dense(1, activation="sigmoid"))
    model.compile(loss="binary_crossentropy", optimizer="adam")

    X = np.random.uniform(-3, 3, size=(10000, 2))
    y = X[:, 0] < X[:, 1]
    loss = model.train_on_batch(X, y)
    name, = model.metrics_names

    print("Single output")
    print(f"{name}\t{loss:.3g}")
    print("")


def experiment_single_output_with_metric():
    model = keras.Sequential()
    model.add(L.Dense(10, activation="elu", input_dim=2))
    model.add(L.Dense(1, activation="sigmoid"))
    model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])

    X = np.random.uniform(-3, 3, size=(10000, 2))
    y = X[:, 0] < X[:, 1]
    metric_outputs = model.train_on_batch(X, y)

    print("Single output with metric")
    for name, value in zip(model.metrics_names, metric_outputs):
        print(f"{name}\t{value:.3g}")
    print("")


def experiment_multiple_outputs():
    input_X = L.Input((2,))
    net = input_X
    net = L.Dense(10, activation="elu")(net)
    output_y = L.Dense(1, activation="sigmoid", name="y1")(net)
    output_z = L.Dense(1, activation="sigmoid", name="y2")(net)
    model = keras.Model(input_X, [output_y, output_z])
    model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])

    X = np.random.uniform(-3, 3, size=(10000, 2))
    y1 = X[:, 0] < X[:, 1]
    y2 = X[:, 0] > X[:, 1]
    metric_outputs = model.train_on_batch(X, [y1, y2])

    print("Multiple outputs")
    for name, value in zip(model.metrics_names, metric_outputs):
        print(f"{name}\t{value:.3g}")
    print("")


if __name__ == "__main__":
    main()
