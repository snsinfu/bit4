import keras
import keras.layers as L
import numpy as np

from layers import AttachClock


window = 6
input_dim = 3
clock_dim = 8

inputs = L.Input((window, input_dim))
outputs = AttachClock(clock_dim)(inputs)
model = keras.Model(inputs, outputs)

X = np.array([
    [[1, 2, 3], [4, 5, 6], [7, 8, 9], [0, 1, 2], [3, 4, 5], [6, 7, 8]],
    [[9, 0, 1], [2, 3, 4], [5, 6, 7], [8, 9, 0], [1, 2, 3], [4, 5, 6]]
])
y = model.predict(X)

np.set_printoptions(precision=3, linewidth=100)
print(y)
