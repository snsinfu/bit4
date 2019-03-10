import keras.backend as K
import keras.layers as L
import numpy as np


class DenseHeads(L.Dense):
    def __init__(self, heads, units, **kwargs):
        super().__init__(heads * units, **kwargs)
        self.heads = heads
        self.head_units = units

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.heads, self.head_units)

    def call(self, inputs):
        output = super().call(inputs)
        shape = K.concatenate([
            K.shape(output)[:-1],
            K.stack([self.heads, self.head_units]),
        ])
        return K.reshape(output, shape)


class AttachClock(L.Layer):
    """
    AttachClock layer attaches clock code to the last axis of input tensor.
    """
    def __init__(self, clock_dim, rate=1, offset=0, **kwargs):
        super().__init__(**kwargs)
        self.clock_dim = clock_dim
        self.rate = rate
        self.offset = 0

    def compute_output_shape(self, input_shape):
        return input_shape[:2] + (input_shape[2] + self.clock_dim,)

    def call(self, inputs):
        batch = K.shape(inputs)[0]
        window = K.int_shape(inputs)[1]

        clock_data = make_clock_data(
            window, self.clock_dim, rate=self.rate, offset=self.offset,
        )
        clock = make_batch(K.constant(clock_data), batch)

        return K.concatenate([inputs, clock])


CLOCK_BASE_RATES = [
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 69,
]


def make_clock_data(window, dim, rate=1, offset=0):
    time = offset + np.linspace(-0.5, 0.5, num=window)
    rates = rate * np.array(CLOCK_BASE_RATES[: (dim + 1) // 2])
    phase = np.multiply.outer(time, rates)
    clock = np.hstack([np.sin(phase), np.cos(phase)])
    return clock[:, :dim]


def make_batch(x, n):
    counts = [n] + [1] * K.ndim(x)
    x = K.expand_dims(x, axis=0)
    x = K.tile(x, K.stack(counts))
    return x
