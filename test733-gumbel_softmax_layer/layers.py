import keras
import keras.backend as K
import keras.layers as L


class GumbelSoftmax(L.Layer):
    def __init__(self, temperature=1, axis=-1, **kwargs):
        super().__init__(**kwargs)
        self.temperature = 1
        self.axis = axis

    def call(self, inputs, training=None):
        u = K.random_uniform(K.shape(inputs))
        u = K.clip(u, K.epsilon(), 1 - K.epsilon())
        noise = -K.log(-K.log(u))
        noised_inputs = (inputs + noise) / self.temperature
        return K.softmax(noised_inputs, axis=self.axis)

    def compute_output_shape(self, input_shape):
        return input_shape
