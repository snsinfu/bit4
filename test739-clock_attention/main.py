import keras
import keras.layers as L
import numpy as np

from layers import AttachClock
from modules import Attention


def main():
    window_size = 30
    input_dim = 2
    clock_dim = 8
    output_dim = 2
    sample_count = 50000

    # Model
    inputs = L.Input((window_size, input_dim))

    net = inputs
    net = L.Conv1D(5, kernel_size=3, padding="same")(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    net = AttachClock(clock_dim)(net)
    net = Attention(keys=16, values=4, heads=5)(net)

    net = L.Flatten()(net)
    net = L.Dense(10)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("tanh")(net)

    net = L.Dense(output_dim)(net)
    outputs = net

    model = keras.Model(inputs, outputs)
    model.compile(loss="mae", optimizer="adam")

    # Training
    freq = np.random.uniform(-20, 20, size=(sample_count, input_dim))
    time = np.linspace(-1, 1, num=(window_size + 1))
    phase = freq[:, None, :] * time[None, :, None]
    data = np.round(16 * np.cos(np.cos(phase)) * np.sin(np.sin(phase)))

    X = data[:, :window_size, :]
    y = data[:, window_size, :]

    train_options = {
        "batch_size": 32,
        "epochs": 10,
        "validation_split": 0.1,
        "verbose": 1,
    }
    model.fit(X, y, **train_options)


if __name__ == "__main__":
    main()
