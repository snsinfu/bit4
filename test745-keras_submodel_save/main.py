import keras
import keras.layers as L


def main():
    inputs = L.Input((10,))
    net = inputs
    net = ResidualFF(10, inner_dim=50)(net)
    net = ResidualFF(10, inner_dim=50)(net)
    net = ResidualFF(10, inner_dim=50)(net)
    net = ResidualFF(10, inner_dim=50)(net)
    net = L.Dense(1)(net)
    net = L.Activation("sigmoid")(net)
    outputs = net
    model = keras.Model(inputs, outputs)

    # Network is OK.
    print(model.summary())

    # Calling to_json raises NotImplementedError. Keras framework does ont support
    # serialization of subclassed models. Even when it is used as a layer in a
    # functional model.
    # https://github.com/keras-team/keras/blob/2.2.4/keras/engine/network.py#L828
    print(model.to_json())


class ResidualFF(keras.Model):
    """
    Feed-forward module with residual connection from input to output.
    """
    def __init__(self, outer_dim, inner_dim=None, activation="relu", dropout=0, **kwargs):
        super().__init__(**kwargs)

        if inner_dim is None:
            inner_dim = outer_dim

        self.inner_dense = L.Dense(inner_dim, name="inner_dense")
        self.inner_norm = L.BatchNormalization(name="nner_norm")
        self.inner_activation = L.Activation(activation, name="inner_activationn")
        self.outer_dense = L.Dense(outer_dim, name="outer_dense")
        self.outer_dropout = L.Dropout(dropout, name="outer_dropout")

    def compute_output_shape(self, input_shape):
        return input_shape[:-1] + (self.outer_dense.units,)

    def call(self, inputs):
        net = inputs
        net = self.inner_dense(net)
        net = self.inner_norm(net)
        net = self.inner_activation(net)
        net = self.outer_dense(net)
        net = self.outer_dropout(net)
        output = inputs + net
        return output


if __name__ == "__main__":
    main()
