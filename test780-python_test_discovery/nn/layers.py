import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


class LayerNormalization(L.Layer):
    """
    Normalizes the last axis of input tensor.
    """

    def __init__(self, epsilon=None, **kwargs):
        super().__init__(**kwargs)

        if epsilon is None:
            epsilon = K.epsilon()
        self.var_epsilon = epsilon ** 2

    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        super().build(input_shape)

    def call(self, x):
        mean = K.mean(x, axis=-1, keepdims=True)
        var = K.var(x, axis=-1, keepdims=True)
        output = (x - mean) / K.sqrt(var + self.var_epsilon)
        output = self.gamma * output + self.beta
        return output
