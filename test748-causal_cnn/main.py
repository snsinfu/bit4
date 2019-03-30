import keras
import keras.layers as L
import numpy as np

from layers import CausalDepthwiseConv1D


# Model definition

timesteps = 30
input_dim = 3
encoding_dim = 20
output_dim = 3

x_seq = L.Input((timesteps, input_dim))

net = x_seq
net = L.Conv1D(encoding_dim, kernel_size=1)(net)

for d in [1, 2, 4, 8]:
    res = net
    net = CausalDepthwiseConv1D(kernel_size=3, depth_multiplier=3, dilation_rate=d)(net)
    net = L.Conv1D(encoding_dim, kernel_size=1)(net)
    net = L.ReLU()(net)
    net = L.add([res, net])

net = L.GlobalAveragePooling1D()(net)
net = L.Dense(output_dim)(net)
net = L.Softmax()(net)
distr = net

model = keras.Model(x_seq, distr)
optimizer = keras.optimizers.Adam(lr=0.01)
model.compile(loss="categorical_crossentropy", optimizer=optimizer)

print(model.summary())
