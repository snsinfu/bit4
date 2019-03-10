import keras
import keras.backend as K
import keras.layers as L

from layers import DenseHeads


class Attention(keras.Model):
    def __init__(self, keys, values, heads=1, **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "use_bias": False,
            "kernel_initializer": "he_normal",
        }
        self.query_dense = DenseHeads(heads, keys, **dense_options)
        self.key_dense = DenseHeads(heads, keys, **dense_options)
        self.value_dense = DenseHeads(heads, values, **dense_options)

    def compute_output_shape(self, input_shape):
        batch, window, *_ = input_shape
        heads = self.value_dense.heads
        values = self.value_dense.head_units
        return batch, window, heads, values

    def call(self, inputs, memory=None):
        if memory is None:
            memory = inputs

        query = self.query_dense(inputs)
        key = self.key_dense(memory)
        value = self.value_dense(memory)

        # query (batch, window, heads, keys)
        # key   (batch, window, heads, keys)
        # --> attention (batch, window, heads, window)
        scale = self.key_dense.head_units ** -0.5
        attention = K.batch_dot(query, key, axes=[3, 3])
        attention = K.softmax(attention * scale)

        # attention (batch, window, heads, window)
        # value     (batch, window, heads, values)
        # --> (batch, window, heads, values)
        output = K.batch_dot(attention, value, axes=[3, 1])

        return output
