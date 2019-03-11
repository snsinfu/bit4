import keras.backend as K
import keras.layers as L


class LayerNormalization(L.Layer):
    def build(self, input_shape):
        shape = (input_shape[-1],)
        self.gamma = self.add_weight(shape=shape, initializer="ones", name="gamma")
        self.beta = self.add_weight(shape=shape, initializer="zeros", name="beta")
        super().build(input_shape)

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, inputs):
        mean = K.mean(inputs, axis=-1, keepdims=True)
        std = K.std(inputs, axis=-1, keepdims=True)
        output = (inputs - mean) / (std + K.epsilon())
        output = self.gamma * output + self.beta
        return output
