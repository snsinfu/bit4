import keras.backend as K
import keras.layers as L


class LayerNormalization(L.Layer):
    def __init__(self, epsilon=None, **kwargs):
        super().__init__(**kwargs)
        self.epsilon = epsilon or K.epsilon()

    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.epsilon)
        output = self.gamma * output + self.beta
        return output
