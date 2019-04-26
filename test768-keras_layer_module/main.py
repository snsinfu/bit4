import logging

import numpy as np
import tensorflow.keras as keras
import tensorflow.keras.layers as L


logging.getLogger("tensorflow").setLevel(logging.ERROR)


class Pass(L.Layer):
    def call(self, x):
        return x


class ResConv1D(L.Layer):
    def __init__(self, filters, kernel_size, activation="elu", dropout=0, **kwargs):
        super().__init__(**kwargs)
        self.conv = L.Conv1D(filters, kernel_size, padding="same")
        self.activation = L.Activation(activation)
        self.dropout = L.SpatialDropout1D(dropout)

    def call(self, x):
        y = self.conv(x)
        y = self.activation(y)
        y = self.dropout(y)
        return x + y


input_window = 10
input_dim = 1

model = keras.Sequential(
    [
        L.InputLayer((input_window, input_dim)),
        ResConv1D(20, kernel_size=3),
        ResConv1D(20, kernel_size=3, dropout=0.5),
        ResConv1D(20, kernel_size=3),
        L.GlobalAveragePooling1D(),
        L.Dense(1, activation="sigmoid"),
    ]
)
model.compile(loss="binary_crossentropy", optimizer=keras.optimizers.Adam(0.05))
print(model.summary())

do_train = False
weight_file = "model.h5"

try:
    model.load_weights(weight_file)
except OSError as e:
    do_train = True

if do_train:
    X = np.random.uniform(-1, 1, size=(100000, input_window, input_dim))
    y = ((X[:, ::2, 0] > X[:, 1::2, 0]).sum(axis=1) >= 3) * 1.0
    model.fit(X, y, batch_size=500, epochs=20, validation_split=0.05)
    model.save_weights(weight_file)

X = np.array([
    [1, 2, 1, 3, 2, 2, 0, 4, 1, 1],
    [2, 1, 3, 1, 2, 2, 4, 0, 1, 1],
])
X = X.reshape(-1, input_window, input_dim)
y = model.predict(X)
print(X[0, :, 0], y[0, 0])
print(X[1, :, 0], y[1, 0])
