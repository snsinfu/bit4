import keras
import keras.backend as K
import keras.layers as L
import numpy as np


def main():
    window_size = 6
    input_dim = 5
    clock_dim = 10

    inputs = L.Input((window_size, input_dim))
    outputs = AttachClock(clock_dim)(inputs)
    model = keras.Model(inputs, outputs)

    X = np.random.uniform(size=(5, window_size, input_dim))
    y = model.predict(X)
    print(np.round(y).astype(int))


class AttachClock(L.Layer):
    def __init__(self, clock_dim, rate=1, **kwargs):
        super().__init__(**kwargs)
        self.clock_dim = clock_dim
        self.rate = rate

    def compute_output_shape(self, input_shape):
        return input_shape[:2] + (input_shape[2] + self.clock_dim,)

    def call(self, inputs):
        input_shape = K.int_shape(inputs)
        window = input_shape[1]

        # FIXME: Hack. Is there any cleaner way?
        clock = K.zeros_like(inputs)
        clock = K.repeat_elements(clock[:, :, :1], self.clock_dim, axis=2)
        clock += K.constant(make_clock(window, self.clock_dim))

        return K.concatenate([inputs, clock])


def make_clock(window, dim, rate=1):
    CLOCK_BASE_RATES = [
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    ]
    time = np.linspace(-0.5, 0.5, num=window)
    rates = rate * np.array(CLOCK_BASE_RATES[: (dim + 1) // 2])
    phase = np.multiply.outer(time, rates)
    clock = np.hstack([np.sin(phase), np.cos(phase)])
    return clock[:, :dim]


if __name__ == "__main__":
    main()
