"""
Neural network modules.
"""
import keras
import keras.backend as K
import keras.layers as L


class InputCNN(keras.Model):
    """
    1D convolutional layers for input feature extraction.
    """
    def __init__(self, filters, kernel_size=3, depth=1, input_dropout=0, **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "use_bias": False,
            "kernel_initializer": "he_normal",
        }
        self.input_dense = L.Dense(filters, **dense_options)
        self.input_norm = L.BatchNormalization()
        self.input_activation = L.Activation("tanh")
        self.input_dropout = L.Dropout(input_dropout)

        conv_options = {
            "padding": "same",
            "use_bias": False,
            "kernel_initializer": "he_normal",
        }
        self.convs = []
        for _ in range(depth):
            conv = L.Conv1D(filters, kernel_size, **conv_options)
            norm = L.BatchNormalization()
            activation = L.Activation("relu")
            self.convs.append((conv, norm, activation))

    def call(self, inputs):
        net = inputs
        net = self.input_dense(net)
        net = self.input_norm(net)
        net = self.input_activation(net)
        net = self.input_dropout(net)

        for conv, norm, activation in self.convs:
            res = net
            net = conv(net)
            net = norm(net)
            net = activation(net)
            net = res + net

        return net

    def compute_output_shape(self, input_shape):
        return input_shape[0], input_shape[1], self.input_dense.units


class Attention(keras.Model):
    """
    Single-head attention.
    """
    def __init__(self, keys, values, **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "use_bias": False,
            "kernel_initializer": "he_normal",
        }
        self.query_dense = L.Dense(keys, **dense_options)
        self.key_dense = L.Dense(keys, **dense_options)
        self.value_dense = L.Dense(values, **dense_options)

    def call(self, inputs, memory=None):
        if memory is None:
            memory = inputs

        q = self.query_dense(inputs)
        k = self.key_dense(memory)
        v = self.value_dense(memory)

        scale = self.key_dense.units ** -0.5
        attention = K.batch_dot(q, k, axes=[2, 2])
        attention = K.softmax(attention * scale)
        output = K.batch_dot(attention, v, axes=[2, 1])

        return output

    def compute_output_shape(self, input_shape):
        return input_shape[0], input_shape[1], self.value_dense.units


class StackedSelfAttentions(keras.Model):
    """
    Stacked, multi-head attentions with residual connections.
    """
    def __init__(self, keys, values, heads, stacks, **kwargs):
        super().__init__(**kwargs)

        dense_options = {
            "use_bias": False,
            "kernel_initializer": "he_normal",
        }
        self.input_dense = L.Dense(values, **dense_options)

        self.attention_heads = []
        for _ in range(heads):
            stack = []
            for _ in range(stacks):
                attention = Attention(keys, values)
                norm1 = L.BatchNormalization()
                dense = L.Dense(values, **dense_options)
                activation = L.Activation("relu")
                norm2 = L.BatchNormalization()
                stack.append((attention, norm1, dense, activation, norm2))
            self.attention_heads.append(stack)

    def call(self, inputs):
        net = inputs
        net = self.input_dense(net)
        pre_attention = net

        outputs = []
        for stacks in self.attention_heads:
            net = pre_attention
            for attention, norm1, dense, activation, norm2 in stacks:
                res = net
                net = attention(net)
                net = norm1(res + net)
                res = net
                net = dense(net)
                net = activation(net)
                net = norm2(res + net)
            outputs.append(net)

        return K.stack(outputs, axis=2)

    def compute_output_shape(self, input_shape):
        return (
            input_shape[0],
            input_shape[1],
            len(self.attention_heads),
            self.input_dense.units,
        )
