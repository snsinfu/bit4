import keras
import keras.layers as L
import numpy as np

from modules import InputCNN, StackedSelfAttentions


window_size = 32
input_dim = 10

inputcnn_depth = 3
hidden_dim = 16
key_dim = 16
attention_heads = 5
attention_stacks = 3
summary_dim = 16
softmax_dim = 8


def main():
    predictor = make_predictor()
    predictor.compile(optimizer="adam", loss="mae")

    # Not really a sequential modeling task but anyway...
    t = np.linspace(-3, 3, num=10000)
    X = np.transpose(np.vstack([
        (np.sin(k * t), np.cos(k * t)) for k in range(input_dim // 2)
    ]))

    train_options = {
        "epochs": 20,
        "verbose": 2,
    }
    train_seqs = keras.preprocessing.sequence.TimeseriesGenerator(
        X, t, window_size, shuffle=True
    )
    predictor.fit_generator(train_seqs, **train_options)


def make_predictor():
    inputs = L.Input((window_size, input_dim))

    net = inputs
    net = InputCNN(hidden_dim, depth=inputcnn_depth)(net)
    net = StackedSelfAttentions(key_dim, hidden_dim, attention_heads, attention_stacks)(net)

    net = L.Reshape((window_size, -1))(net)
    net = L.Dense(summary_dim)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("relu")(net)

    net = L.Flatten()(net)
    net = L.Dense(softmax_dim)(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("softmax")(net)

    net = L.Dense(1)(net)
    outputs = net

    return keras.Model(inputs=inputs, outputs=outputs)


if __name__ == "__main__":
    main()
