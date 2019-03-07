import keras
import keras.layers as L
import numpy as np

from layers import MyTimeDistributed


window = 10
channels = 3

inputs = L.Input((channels,))
net = inputs
net = L.Dense(1)(net)
outputs = net
transform = keras.Model(inputs, outputs)

inputs = L.Input((window, channels))
outputs = MyTimeDistributed(transform)(inputs)
model = keras.Model(inputs, outputs)
model.compile(loss="mae", optimizer=keras.optimizers.SGD(0.01, nesterov=True))

x = np.random.uniform(0, 1, size=(100, window, channels))
y = np.mean(x, axis=2).reshape(x.shape[:2] + (1,))
model.fit(x, y, epochs=40, batch_size=4)

# NOTE: Setting batch_size ~ 16 or larger messes up training. The reason is not
# clear to me but the way MyTimeDistributed works should have something to do with.
# It treats input window as a batch so the effective batch size becomes large, which
# can be bad for SGD training.

x = np.array([[0.1, 0.2, 0.3]])
y = transform.predict(x)
print(y)
