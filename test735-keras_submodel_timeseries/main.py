import keras
import keras.backend as K
import keras.layers as L
import matplotlib.pyplot as plt
import numpy as np


def main():
    window_size = 10
    input_dim = 3
    hidden_dim = 10
    kernel_size = 5
    output_dim = 3

    # Model
    transform = GatedDense(1, activation="tanh")

    inputs = L.Input((window_size, input_dim))
    net = L.TimeDistributed(transform)(inputs) # XXX: This is the key

    res = net
    net = L.Conv1D(hidden_dim, kernel_size, padding="same")(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("relu")(net)
    net = L.add([res, net])

    res = net
    net = L.Conv1D(hidden_dim, kernel_size, padding="same")(net)
    net = L.BatchNormalization()(net)
    net = L.Activation("relu")(net)
    net = L.add([res, net])

    net = L.Dense(output_dim)(net)
    net = L.Activation("softplus")(net)
    outputs = net

    model = keras.Model(inputs, outputs)
    model.compile(loss="mae", optimizer="adam")

    # Training
    jet = plt.get_cmap("jet")
    viridis = plt.get_cmap("viridis")

    x = np.linspace(0, 1, num=1000)
    input_seq = jet(x)[:, :3]
    output_seq = viridis(x)[:, :3]
    train_data = Seq2seqGenerator(input_seq, output_seq, window_size)

    model.fit_generator(train_data, epochs=50, verbose=2)

    # Use the transform submodel. This works thanks to the TimeDistributed wrapper.
    inputs = L.Input((input_dim,))
    transform_model = keras.Model(inputs, transform(inputs))
    test_X = jet(np.linspace(0, 1, num=10))[:, :3]
    test_y = transform_model.predict(test_X)
    print(test_y)


class GatedDense(keras.Model):
    def __init__(self, dim, activation="relu", **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "use_bias": False,
            "kernel_initializer": "he_normal"
        }
        self.feature_dense = L.Dense(dim, **dense_options)
        self.feature_norm = L.BatchNormalization()
        self.feature_activation = L.Activation(activation)
        self.gate_dense = L.Dense(dim, **dense_options)
        self.gate_norm = L.BatchNormalization()
        self.gate_activation = L.Activation("sigmoid")

    def call(self, inputs):
        feature = self.feature_dense(inputs)
        feature = self.feature_norm(feature)
        feature = self.feature_activation(feature)
        gate = self.gate_dense(inputs)
        gate = self.gate_norm(gate)
        gate = self.gate_activation(gate)
        return feature * gate

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.feature_dense.units,)


class Seq2seqGenerator(keras.utils.Sequence):
    def __init__(self, x_seq, y_seq, window_size, batch_size=32):
        self.x_seq = x_seq
        self.y_seq = y_seq
        self.window_size = window_size
        self.batch_size = batch_size

    def __len__(self):
        return (len(self.x_seq) - self.window_size + 1) // self.batch_size

    def __getitem__(self, idx):
        begs = np.random.randint(len(self.x_seq) - self.window_size, size=self.batch_size)
        ends = begs + self.window_size
        batch_xs = []
        batch_ys = []
        for beg, end in zip(begs, ends):
            batch_xs.append(self.x_seq[beg:end])
            batch_ys.append(self.y_seq[beg:end])
        batch_xs = np.array(batch_xs)
        batch_ys = np.array(batch_ys)
        return batch_xs, batch_ys


if __name__ == "__main__":
    main()
