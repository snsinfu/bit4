import tensorflow.keras.backend as K
import tensorflow.keras.layers as L


class LayerNormalization(L.Layer):
    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        self.var_epsilon = K.epsilon() ** 2
        super().build(input_shape)

    def call(self, inputs):
        mean = K.mean(inputs, axis=-1, keepdims=True)
        var = K.var(inputs, axis=-1, keepdims=True)
        output = (inputs - mean) / K.sqrt(var + self.var_epsilon)
        output = self.gamma * output + self.beta
        return output
