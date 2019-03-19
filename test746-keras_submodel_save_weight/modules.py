import keras
import keras.layers as L


def as_activation_layer(activation):
    """
    Return an activation layer specified by the argument.
    """
    if isinstance(activation, L.Layer):
        return activation
    return L.Activation(activation)


class ResidualFC(keras.Model):
    """
    Fully-connected module with residual connection from input to output.
    """
    def __init__(
        self, outer_dim, inner_dim=None, activation="relu", dropout=0, **kwargs
    ):
        super().__init__(**kwargs)

        if inner_dim is None:
            inner_dim = outer_dim

        self.inner_dense = L.Dense(inner_dim)
        self.inner_activation = as_activation_layer(activation)
        self.outer_dense = L.Dense(outer_dim)
        self.dropout = L.Dropout(dropout)
        self.batchnorm = L.BatchNormalization()

    def compute_output_shape(self, input_shape):
        return input_shape

    def call(self, x):
        net = x
        net = self.inner_dense(net)
        net = self.inner_activation(net)
        net = self.outer_dense(net)
        net = self.dropout(net)
        output = self.batchnorm(x + net)
        return output
