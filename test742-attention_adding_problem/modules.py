import keras
import keras.backend as K
import keras.layers as L

from layers import LayerNormalization


def scaled_dot_product_attention(q, k, v):
    """
    q (batch, heads, query_window, key_dim)
    k (batch, heads, memory_window, key_dim)
    v (batch, heads, memory_window, value_dim)
    """
    key_dim = K.int_shape(k)[-1]

    # (batch, heads, query_window, memory_window)
    attention = K.batch_dot(q, k, axes=[3, 3])
    attention = K.softmax(attention / key_dim ** 0.5)

    # (batch, heads, query_window, value_dim)
    output = K.batch_dot(attention, v, axes=[3, 2])

    return output


def split_heads(x, heads):
    """
    x (batch, window, heads * dim) --> (batch, heads, window, dim)
    """
    window, data_dim = K.int_shape(x)[1:]
    dim = data_dim // heads
    x = K.reshape(x, (-1, window, heads, dim))
    x = K.permute_dimensions(x, (0, 2, 1, 3))
    return x


def join_heads(x):
    """
    x (batch, heads, window, dim) --> (batch, window, heads * dim)
    """
    heads, window, dim = K.int_shape(x)[1:]
    x = K.permute_dimensions(x, (0, 2, 1, 3))
    x = K.reshape(x, (-1, window, heads * dim))
    return x


class Attention(keras.Model):
    def __init__(self, keys, values, heads=1, **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "kernel_initializer": "he_normal"
        }
        self.query_dense = L.Dense(keys * heads, **dense_options)
        self.key_dense = L.Dense(keys * heads, **dense_options)
        self.value_dense = L.Dense(values * heads, **dense_options)
        self.output_dense = L.Dense(values, **dense_options)
        self.keys = keys
        self.values = values
        self.heads = heads

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.values,)

    def call(self, inputs, memory=None):
        if memory is None:
            memory = inputs

        query = split_heads(self.query_dense(inputs), self.heads)
        key = split_heads(self.key_dense(memory), self.heads)
        value = split_heads(self.value_dense(memory), self.heads)

        output = scaled_dot_product_attention(query, key, value)
        output = join_heads(output)
        output = self.output_dense(output)

        return output


class AttentionEncoder(keras.Model):
    def __init__(
        self,
        keys,
        values,
        heads=1,
        inner_dim=None,
        latent_dropout=0,
        output_dropout=0,
        **kwargs
    ):
        super().__init__(**kwargs)

        if inner_dim is None:
            inner_dim = values

        dense_options = {
            "kernel_initializer": "he_normal"
        }
        self.attention = Attention(keys, values, heads)
        self.latent_dropout = L.Dropout(latent_dropout)
        self.latent_norm = LayerNormalization()
        self.inner_dense = L.Dense(inner_dim, **dense_options)
        self.inner_activation = L.Activation("relu")
        self.output_dense = L.Dense(values, **dense_options)
        self.output_dropout = L.Dropout(output_dropout)
        self.output_norm = LayerNormalization()

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, inputs):
        latent = self.attention(inputs)
        latent = self.latent_dropout(latent)
        latent = self.latent_norm(inputs + latent)
        pre_output = self.inner_dense(latent)
        pre_output = self.inner_activation(pre_output)
        output = self.output_dense(pre_output)
        output - self.output_dropout(output)
        output = self.output_norm(latent + output)
        return output
